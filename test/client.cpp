#include "../src/socket.h"
#include <iostream>

int main(){
    Socket* s = new Socket();
    int ret;
    ret = s->Connect("127.0.0.1:9000");
    if (ret < 0){
        return -1;
    }
    
    ret = s->Send("Hello from client");
    if (ret < 0){
        return -1;
    }

    std::cout << "Hello message sent" << std::endl;
    
    ret = s->Recev();
    if (ret < 0){
        return -1;
    }
    std::cout << s->ReadBuf() << std::endl;

    return 0;
}