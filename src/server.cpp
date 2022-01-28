#include "server.h"
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>

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
    //initialize...
    event = (struct epoll_event*) malloc(sizeof(struct epoll_event*));
    event->data.fd = listen_socket->Fd();
    event->events = EPOLLIN | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listen_socket->Fd(), event) < 0) {
        abort();
    }
    events = (struct epoll_event*) calloc(MAXEVENTS, sizeof(struct epoll_event*));
}

int Server::Run(){
    std::cout << "Blocking and wait for epoll event..." << std::endl;
    int n;
    while(1){
        //specifying timeout equals 0 causes epoll_wait to  return immediately, even if no events are available.
        //blocking timeout here...
        n = epoll_wait(efd, events, MAXEVENTS, 0);
        for(int i = 0; i < n; i++){
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (events[i].events &  EPOLLRDHUP)){
                //closing  a socket fd causes it to be removed from epoll sets...
                close(events[i].data.fd);
            } else if((events[i].events & EPOLLIN)){
                //connect event...
                if (events[i].data.fd == listen_socket->Fd()){
                    int new_fd;
                    while(1){
                        new_fd = listen_socket->Accept();
                        if (new_fd < 0){
                            //have accepted all connections... 
                            break;
                        }
                        Socket* new_socket = new Socket(new_fd);
                        new_socket->SetSocketNonBlocking();
                        event->data.fd = new_fd;
                        event->events = EPOLLIN | EPOLLET;
                        if(epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, event) < 0){
                            std::cout << "failed to add fd into epoll event" << std::endl;
                        }
                    }
                } else {
                    //IO event...
                    Socket* s = new Socket(events[i].data.fd);
                    //about the buf size, there is a balance here...
                    int n = s->Recev();
                    if (n < 0){
                        continue;
                    }   

                    char* buf = s->ReadBuf();
                    buf[n] = '\0';
                    //call Handler...
                    char* toWrite = Handler(buf);
                    //error happened, close fd...
                    if(s->Send(toWrite) < 0){
                        close(events[i].data.fd);
                    }
                }
            } else{
                //TODO...
                continue;
            }
            
        }
    }
}

Server::~Server(){
    delete listen_socket;
    delete []events;
    delete efd;
}