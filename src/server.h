#include "socket.h"
#include <string>

enum EventType {IOEVENT, TIMEEVENT, SIGNALEVENT};

//to be optimized...
const int MAXEVENTS = 1024;

class Server{
    public:
        Server(int port = 8080);
        ~Server();
        //IO Handle fun...
        char* (*Handler)(char* buf);
        //run server...
        int Run();
        int AddTimerEvent();
    private:
        int efd;
        struct epoll_event* event;
        struct epoll_event *events;
        Socket* listen_socket;
        Socket* timer_socket;
};