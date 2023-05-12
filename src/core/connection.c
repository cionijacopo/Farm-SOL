/*
    Created by Jacopo Cioni
    FarmProject - SOL
    File sorgente contenente il funzionamento della connect e della read/write.
*/

#include"../../includes/connection.h"

#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/un.h>
#include<sys/socket.h>

extern struct sockaddr_un *socket_add; 

void cleanup() {
    unlink(SOCKETNAME);
}

int clientSocket() {
    // Ottengo il file descriptor del socket
    int fd_client = socket(AF_UNIX, SOCK_STREAM, 0);
    int riprova = 3;
    // Controllo che sia tutto ok
    if(fd_client == -1) {
        perror("SOCKET");
        return -1;
    }
    // TEST: --> OK
    // printf("Fd_client: %d\n", fd_client);  
    // Ritento la connessione più volte
    while(connect(fd_client, (struct sockaddr*)&socket_add, sizeof(struct sockaddr_un)) == -1 && riprova > 0) {
        if(errno == ENOENT) {
            // riprovo
            printf("Socket non trovato.\n"); 
            sleep(3); 
        } else if(errno == ECONNREFUSED) {
            riprova--;
            sleep(3);
        } else { 
            perror("CONNECT");
            exit(EXIT_FAILURE);
        }
    }
    if(riprova == 0) {
        return -1;
    }
    // Ho il file descriptor del client socket
    return fd_client;
}

int readn(long fd, void *buf, size_t size) {
    size_t left = size;
    int r;
    char *bufptr = (char*)buf;
    while(left>0) {
	if ((r=read((int)fd ,bufptr,left)) == -1) {
	    if (errno == EINTR) continue;
	    return -1;
	}
	if (r == 0) return 0;   // EOF
        left    -= r;
	bufptr  += r;
    }
    return size;
}

int writen(long fd, void *buf, size_t size) {
    size_t left = size;
    int r;
    char *bufptr = (char*)buf;
    while(left>0) {
	if ((r=write((int)fd ,bufptr,left)) == -1) {
	    if (errno == EINTR) continue;
	    return -1;
	}
	if (r == 0) return 0;  
        left    -= r;
	bufptr  += r;
    }
    return 1;
}
