#include <iostream>
#include <fcntl.h>
#include "socket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>

Socket::Socket(){
    //only support ipv4...
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in  a = {0};
    address = &a;
}

Socket::Socket(int fd){
    socket_fd = fd;
    struct sockaddr_in a = {0};
    address = &a;
}

Socket::~Socket(){
    if(read_buf){
        read_buf[1023] = '\0';
        delete [] read_buf;
    }
    
    delete address;

    close(socket_fd);
}

int Socket::SetAddrPortReuse(){
    if (socket_fd < 0 ){
        return -1;
    }
    //set socket opt...
    int opt = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        return -1;
    }
    return 0;
}

int Socket::Bind(int port){
    if (socket_fd < 0 ){
        return -1;
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
    int ret;
    ret  = bind(socket_fd, (struct sockaddr *) address, sizeof(*address));
    if (ret <  0) {
        std::cout << "Failed to bind a port" << std::endl;
        return -1;
    }
    return 0;
}

int Socket::Listen(){
    if (socket_fd < 0 ){
        return -1;
    }
    int ret = listen(socket_fd, SOMAXCONN);
    if (ret < 0){
        std::cout << "failed to listen" << std::endl;
        return -1;
    }
    return 0;
}

int Socket::Accept(){
    if (socket_fd < 0 ){
        return -1;
    }
    int new_socket;
    new_socket = accept(socket_fd, (struct sockaddr *) address, (socklen_t*)address);
    if (new_socket < 0) {
        std::cout << "failed to accept new fd" << std::endl;
        return -1;
    }
    return new_socket;
}

int Socket::SetSocketNonBlocking(){
    if (socket_fd < 0 ){
        return -1;
    }
    int ret = fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);
    if(ret < 0){
        std::cout << "failed to set socket non blocking" << std::endl;
        return -1;
    }
    return 0;
}

int Socket::Recev(){
    int num = read(socket_fd, read_buf, sizeof(read_buf));
    if (num < 0) {
        std::cout << "failed to read data from fd" << std::endl;
        return -1;
    }
    return num;
}

int Socket::Send(char* msg){
    int ret = send(socket_fd, msg, strlen(msg), 0);
    if (ret < 0) {
        std::cout << "failed to send data into fd" << std::endl;
        return -1;
    }
    return ret;
}

int Socket::Connect(std::string addr){  
    int pos = addr.find(":");
    std::string ip = addr.substr(0, pos);
    std::string port = addr.substr(pos+1);
    int port_int = std::stoi(port);
    // std::cout << "ip is " << ip << std::endl;
    // std::cout << "port is " << port_int << std::endl;
    address->sin_family = AF_INET;
    address->sin_port = htons(port_int);
    if(inet_pton(AF_INET, ip.c_str(), &address->sin_addr) <= 0){
        std::cout << "Invalid address / Address not supported" << std::endl;
        return -1;
    }
    if(connect(socket_fd, (struct sockaddr *) address, sizeof(*address)) < 0){
        std::cout << "Failed to connect" << std::endl;
        return -1;
    }
    return 0;
}
