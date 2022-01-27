
#include <sys/socket.h>
#include <string>

class Socket {
    public:
        Socket();
        Socket(int fd);
        ~Socket();
        int SetAddrPortReuse();
        int Bind(int port);
        int Listen();
        int Accept();
        int Connect(std::string addr);
        int Send(char* msg);
        int Recev();
        int SetSocketNonBlocking();
        int Fd(){
            return socket_fd;
        }
        char* ReadBuf(){
            return read_buf;
        }
        
    private:
        int socket_fd;
        struct sockaddr_in* address;
        char read_buf[1024];
};