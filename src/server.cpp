#include "server.h"
#include <sys/epoll.h>

Server::Server(int port){
    efd = epoll_create1(0);
    if (efd == -1) {
        throw("epoll create error");
        abort();
    }
    listen_socket = new Socket();
    if(listen_socket->SetAddrPortReuse() < 0){
        abort();
    }
    if(listen_socket->SetSocketNonBlocking() < 0){
        abort();
    }
    if(listen_socket->Bind(port) < 0){
        abort();
    }
    if(listen_socket->Listen() < 0 ){
        abort();
    }
    event->data.fd = listen_socket->Fd();
    event->events = EPOLLIN | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listen_socket->Fd(), event) < 0) {
        abort();
    }
    events = (struct event*) calloc(MAXEVENTS, sizeof(*event));
}

int Server::Run(){
    int n;
    std::cout << "Blocking and wait for epoll event..." << std::endl;
    
    while(1){
        //specifying timeout equals 0 causes epoll_wait to  return immediately, even if no events are available.
        //blocking timeout here...
        n = epoll_wait(efd, events, MAXEVENTS, 0);
        for(int i = 0; i < n; i++){
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (events[i].events &  EPOLLRDHUP)){
                close(events[i].data.fd)
            } else if((events[i].events & EPOLLIN)){
                //connect event...
                if ((events[i].data.fd == listen_socket->Fd()){
                    int new_fd;
                    while(1){
                        new_fd = listen_socket->Accept();
                        if (new_fd < 0){
                            //have accepted all connections...
                            break;
                        }
                        Socket* new_socket = new Socket(new_fd);
                    }
                } else{
                    ;
                }
            } else{
                ;
            }
            
        }

    }
}

Server::~Server(){
    delete listen_socket;
    del []events;
}