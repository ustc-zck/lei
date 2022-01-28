#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
using namespace std;

void doSomething(int id) {
    cout << id << "\n";
}

/**
 * Spawns n threads
 */

int main()
{
    int n = 10;
     std::vector<thread> threads(n);
    // spawn n threads:
    while(1){
        std::cout << "into loop..." << std::endl;
         for (int i = 0; i < n; i++) {
            threads[i] = std::thread(doSomething, i + 1);
        }
        for (auto& th : threads) {
            th.join();
        }
        std::cout << "out of loop..." << std::endl;
    }

  
   

    std::cout << "program end" << std::endl;

    return 0;
}