#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "md5.h"
#include "wisdom.h"

#define LISTEN_BACKLOG 10

// function prototypes
int create_socket(void);
SOCKET_ADDRESS create_socket_address(int port);
void bind_address_to_socket(int sock, SOCKET_ADDRESS addr);
void start_to_listen(int sock, int backlog);
int accept_client_socket(int server_sock, SOCKET_ADDRESS *client_addr);
void do_work(int client_sock, SOCKET_ADDRESS client_addr);
void cleanup_when_done(void);
char *get_proverb(void);

// main program
int main(int argc, char **argv) {
    int server_sock, client_sock;
    SOCKET_ADDRESS server_addr, client_addr;

    server_sock = create_socket();
    server_addr = create_socket_address(SERVER_PORT);
    bind_address_to_socket(server_sock, server_addr);
    start_to_listen(server_sock, LISTEN_BACKLOG);

    // prints message and then waits for and proccesses client requests 
    // until Ctrl-C is pressed
    printf("Words of Wisdom service started. Press Ctrl-C to terminate.\n");
    while (true) {
        client_sock = accept_client_socket(server_sock, &client_addr);

        if (fork() == 0) {
            // do actual work in the child process
            do_work(client_sock, client_addr);
        } else { 
            // original process
            cleanup_when_done();
        }

        close(client_sock);
    }

    close(server_sock);
    return EXIT_SUCCESS;
}

int create_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sock;
}

SOCKET_ADDRESS create_socket_address(int port) {
    SOCKET_ADDRESS addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // must transform port from host order to network order
    addr.sin_addr.s_addr = INADDR_ANY;
    return addr;
}

void bind_address_to_socket(int sock, SOCKET_ADDRESS addr) {
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void start_to_listen(int sock, int backlog) {
    if (listen(sock, LISTEN_BACKLOG) != 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int accept_client_socket(int server_sock, SOCKET_ADDRESS *client_addr) {
    int sock;
    int addrlen = sizeof(*client_addr);
    sock = accept(server_sock, (struct sockaddr *)client_addr, &addrlen);
    if (sock < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void do_work(int client_sock, SOCKET_ADDRESS client_addr) {
    char user_name[MAX_USER_NAME_LENGTH];
    char addr_str[INET_ADDRSTRLEN];
    char challenge[MAX_CHALLENGE_LENGTH];
    char input_to_hash[MAX_MESSAGE_LENGTH];
    char hash[MD5_STRING_LENGTH];
    char response[MD5_STRING_LENGTH];
    char *proverb;

    memset(user_name, 0, MAX_USER_NAME_LENGTH);
    read(client_sock, user_name, MAX_USER_NAME_LENGTH);
    inet_ntop(AF_INET, &(client_addr.sin_addr), addr_str, INET_ADDRSTRLEN);
    printf("[MESG] %s connected from: %s\n", user_name, addr_str);

    srandom((unsigned int)getpid());
    sprintf(challenge, "%ld", random());
    write(client_sock, challenge, strlen(challenge));
    printf("[SENT] %s\n", challenge);

    // hash = md5(challenge + user_name + SHARED_SECRET_KEY)
    strcpy(input_to_hash, challenge);
    strcat(input_to_hash, user_name);
    strcat(input_to_hash, SHARED_SECRET_KEY);
    md5(input_to_hash, hash);

    memset(response, 0, MD5_STRING_LENGTH);
    read(client_sock, response, MD5_STRING_LENGTH);
    printf("[RECV] %s\n", response);

    if (strcmp(hash, response) == 0) {
        proverb = get_proverb();
        write(client_sock, proverb, strlen(proverb));
        printf("[MESG] %s authenticated.\n", user_name);
    } else {
        write(client_sock, "I don't know you. I have no advice for you.", MAX_MESSAGE_LENGTH);
        printf("[MESG] Authentication failed.\n");
    }
}

void cleanup_when_done(void) {
    int cpid, status;
    while ((cpid = waitpid(-1, &status, WNOHANG)) > 0);
    if (cpid < 0 && errno != ECHILD) { 
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
}
