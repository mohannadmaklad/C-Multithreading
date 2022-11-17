#include <iostream>
#include <thread>
#include <chrono>

void thread_function(){
    std::cout << "from thread" << std::endl;
}

int main(){
    std::thread t1(thread_function);
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    std::cout << "from main\n";
    //t1.join(); //wait for t1 to finish
    t1.detach(); //t1 will run on its own, c++ rtl will handle its termination
    if(t1.joinable()){
        t1.join();
    }else{
        std::cout << "cannot join after detaching" << std::endl;
    }
    return 0;
}