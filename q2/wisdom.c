// Use the command "make client" to compile this program.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "md5.h"
#include "wisdom.h"

#define MAXDATASIZE 200		// The maximum number of bytes we can get at one time
  int main (int argc, char **argv)
{
  
    // Your code
int sockfd, numbytes;
  
char challenge2[MAX_CHALLENGE_LENGTH];
  
char response[MD5_STRING_LENGTH];
  
char hash[MD5_STRING_LENGTH];
  
char input[MAXDATASIZE];
  
char auth[MAX_MESSAGE_LENGTH];
  
struct hostent *srv_name;
  
struct sockaddr_in srv_info;	// define structure server address information 
struct sockaddr_in clnt_info;	// define structure client address information 
  
if (argc != 4)                  // Control the number of input values
    {
      fprintf (stderr,"Usage: wisdom server_ip_address user_name password\n");
      
exit (1);
    
}
  
 
if ((srv_name = gethostbyname (argv[1])) == NULL)  // get host information
    {				
      perror ("gethostbyname error");
      
exit (1);
    
}
  
 

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)  //Create socket
    {
      perror ("socket creation error");
      
exit (1);
    
}
  
    
    //connect pre-preparation
  srv_info.sin_family = AF_INET;	 
  srv_info.sin_port = htons (SERVER_PORT); 
  srv_info.sin_addr = *((struct in_addr *) srv_name->h_addr);
  
    memset (&(srv_info.sin_zero), '\0', 8);	// Rest struct 
  
   // Connnet() to server
    if (connect(sockfd, (struct sockaddr *) &srv_info,sizeof (struct sockaddr)) == -1)
    {
      perror ("connect error");
      
exit (1);
    
}
  
 
    //
    // send(... user_name ...);
    if (send (sockfd, argv[2], MAX_USER_NAME_LENGTH, 0) == -1)
    {
      perror ("write error");
      
exit (1);
    
}
  printf ("[SENT] %s\n", argv[2]);
  
 
    // ...
    // recv(... challenge ...);
    memset (challenge2, 0, MAX_CHALLENGE_LENGTH);
  
if (recv (sockfd, challenge2, MAX_CHALLENGE_LENGTH, 0) == -1)
    {
      perror ("read error");
      
exit (1);
    
}
  
    printf ("[RECV] %s\n", challenge2);
  
 
    // ...
    // input = challenge + user_name + password
    strcpy (input, challenge2);
    strcat (input, argv[2]);
    strcat (input, argv[3]);
  
    // ...
    // Makes an MD5 hash of input and puts it in output, where
    // input and response are both char[]. 
    md5 (input, response);
  
 
if (write (sockfd, response, MD5_STRING_LENGTH) == -1)
    {
      perror ("response error");
      
exit (1);
    
}
  
    printf ("[SENT] %s\n", response);
  
 
    //recv(...authentication result...);
    memset (auth, 0, MAX_MESSAGE_LENGTH);
  
if (recv (sockfd, auth, MAX_MESSAGE_LENGTH, 0) == -1)
    {
     perror ("read error");
      
exit (1);
    
}
  
    printf ("[RECV] %s\n", auth);
  
 
close (sockfd);
  
return 0;

}


