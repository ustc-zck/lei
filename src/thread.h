#include <iostream>
#include <thread>

class Thread{
    public:
        Thread();
        ~Thread();
        int GetId();
        bool Joinable();
        bool Join();
        void Detach();
    private:
        std::thread thread;
}