#include "socket.h"

enum EventType {IOEVENT, TIMEEVENT, SIGNALEVENT};

//to be optimized...
const int MAXEVENTS = 1024;

class Server{
    public:
        Server(int port = 8080);
        ~Server();
        //set io process func...
        void SetIOProc();
        //run server...
        int Run();
    private:
        int efd;
        struct epoll_event* event;
        struct epoll_event *events;
        Socket* listen_socket;
};