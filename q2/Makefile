all: server client md5

server: server.c wisdom.h md5.h md5.c proverbs.c
	gcc -o server server.c wisdom.h md5.h md5.c proverbs.c

client: wisdom.c wisdom.h md5.h md5.c
	gcc -o wisdom wisdom.c wisdom.h md5.h md5.c

md5: md5.h md5.c
	gcc -c md5.c md5.h

clean: 
	rm -f server wisdom *.o

