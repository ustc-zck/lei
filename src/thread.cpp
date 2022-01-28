#include <iostream>

Thread::Thread(){
}

Thread::~Thread(){
    delete thread;
}

int Thread::GetId(){
    return thread.get_id();
}

bool Thread::Joinable(){
    return thread.joinable();
}

void Thread::Detach(){
    return  thread.detach():
}





