#include <iostream>
#include <fcntl.h>
#include "socket.h"
#include <arpa/inet.h>

//only support ipv4...
Socket::Socket(){
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd){
    socket_fd = fd;
}

int Socket::Bind(int port){
    struct sockaddr_in* address;
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
    int ret;
    ret  = bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr*));
    if (ret != 0) {
        return -1;
    }
    return 0;
}

int Socket::Listen(){
    
}

int Socket::SetSocketNonBlocking(){
    if (socket_fd < 0 ){
        return -1;
    }
    int ret = fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);
    if(ret < 0){
        return -1;
    }
    return 0;
}

