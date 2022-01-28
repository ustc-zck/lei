#include "socket.h"
#include <string>
#include <vector>

#include <sys/epoll.h>
#include <unistd.h>
#include <sys/timerfd.h>

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
        //wrapper...
        void Wrapper(int fd){
            Socket* s = new Socket(fd);
            //about the buf size, there is a balance here...
            int n = s->Recev();
            if (n < 0){
                close(fd);
                return;
            }   
            char* buf = s->ReadBuf();
            buf[n] = '\0';
            //call Handler...
            char* toWrite = Handler(buf);
            //error happened, close fd...
            if(s->Send(toWrite) < 0){
                close(fd);
            }
        }
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