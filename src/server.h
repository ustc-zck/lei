#include "socket.h"
#include <string>
#include <vector>
enum EventType {IOEVENT, TIMEEVENT, SIGNALEVENT};


//to be optimized...
const int MAXEVENTS = 1024;

class Server{
    public:
        Server(int port = 8080);
        ~Server();
        //IO Handle func...
        char* (*Handler)(char* buf);
        //IO Timer func...
        int (*TimeHandler)();
        //Add time event...
        int AddTimeEvent(int millisceonds);
        //run server...
        int Run();
    private:
        //event fd...
        int efd;
        struct epoll_event* event;
        struct epoll_event *events;
        Socket* listen_socket;

        Socket* timer_socket;
};