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
                        new_socket->SetSocketNonBlocking();
                        event.data.fd = new_fd;
                        event.events = EPOLLIN | EPOLLET;
                        if(epoll_ctl(efd, EPOLL_CTL_ADD, new_fd, &event) < 0){
                            std::cout << "failed to add fd into epoll event" << std::endl;
                            abort();
                        }
                    }
                } else {
                    //IO event...
                    Socket* s = new Socket(events[i].data.fd);
                    //adopt a maxmium buf size...
                    //about the buf size, there is a balance here...
                    int n = s->Recev();
                    if (n < 0){
                        continue;
                    }

                    char* buf = s->ReadBuf();
                    buf[n] = '\0';
                    std::string str = buf;
                    //error happened, close fd...
                    if(s->Send(Handler(buf)) < 0){
                        if(epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]) < 0){
                            aort();
                        }
                    }
                    delete s;
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