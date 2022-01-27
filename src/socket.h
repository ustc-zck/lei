#include <iostream>
#include <sys/socket.h>

class Socket {
    public:
        Socket();
        Socket(int fd);
        int Bind(int port);
        int Listen();
        int Accept();
        int Connect();
        int Send();
        int Recev();
        int SetSocketNonBlocking();
    private:
        int socket_fd;
        char read_buf[1024];
        char write_buf[1024];
};