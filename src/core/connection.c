/*
    Created by Jacopo Cioni
    FarmProject - SOL
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

/*
struct sockaddr_un socket_address;
socklen_t address_length;


void initServerAddress() {
    memset(&socket_address, '0', sizeof(socket_address));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, SOCKETNAME, strlen(SOCKETNAME)+1);
}
*/

void cleanup() {
    unlink(SOCKETNAME);
}

/*
int serverSocket(int *fd_socket, struct sockaddr_un *socket_add) {
    // Ottengo il file descriptor del socket
    (*fd_socket) = socket(AF_UNIX, SOCK_STREAM, 0);
    // Controllo che sia tutto ok
    if((*fd_socket) == -1) {
        perror("SOCKET");
        cleanup();
        return -1;
    }
    // Faccio il binding 
    if((bind((*fd_socket), (struct sockaddr *)socket_add, sizeof(socket_add))) != 0) {
        perror("BIND");
        close((*fd_socket));
        cleanup();
        return -1;
    }
    if((listen((*fd_socket), MAX_BACK_LOG)) != 0) {
        perror("LISTEN");
        close((*fd_socket));
        cleanup();
        return -1;
    }
    return 0;
}
*/

int clientSocket() {
    // Ottengo il file descriptor del socket
    int fd_client = socket(AF_UNIX, SOCK_STREAM, 0);
    // Controllo che sia tutto ok
    if(fd_client == -1) {
        perror("SOCKET");
        return -1;
    }
    // TEST: --> OK
    // printf("Fd_client: %d\n", fd_client);  
    // Ritento la connessione più volte
    while(connect(fd_client, (struct sockaddr*)&socket_add, sizeof(struct sockaddr_un)) == -1) {
        if(errno == ENOENT) {
            // riprovo
            printf("Socket non trovato.\n"); 
            sleep(3); 
        } else {
            perror("CONNECT");
            exit(EXIT_FAILURE);
        }
    }
    // Ho il file descriptor del client socket
    return fd_client;
}

/*
int accettaClient(int fd_socket) {
    int fd_client;
    fd_client = accept(fd_socket, (struct sockaddr *)&socket_address, &address_length);
    return fd_client;
}
*/
/*
int readn(long fd, void *buf, size_t size) {
    size_t left = size;
    int r;
    char *bufptr = (char*) buf;
    while (left > 0) {
        // Read ritorna il numero di elementi letti.
        if ((r = read((int)fd, bufptr, left)) == -1) {
            if (errno = EINTR) continue;
            return -1;
        }
        if (r == 0) {
            // Sono alla fine del file.
            return 0; 
        }
        // tolgo i byte appena letti
        left = left -r;
        // scorro il buffer tanto quanti byte ho appena letto
        bufptr = bufptr + r;    
    }
    return size;
}

int writen(long fd, void *buf, size_t size) {
    size_t left = size;
    int r;
    char *bufptr = (char*) buf;
    while(left > 0) {
        if((r = write((int)fd, bufptr, left)) == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        if(r == 0) {
            return 0;
        }
        left = left -r;
        bufptr = bufptr - r;
    }
    return -1; 
}
*/

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
