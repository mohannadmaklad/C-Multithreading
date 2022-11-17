#include <iostream>
#include <chrono>
#include <thread>


void thread_1(){
    for(auto i=100; i>0; --i){
        std::cout << "--------------From function thread \n";
    }
}

struct ftor_thread{
    void operator()(){
        for(auto i=100; i>0; --i){
            std::cout << "+++++++++++++From functor thread \n";
        }
    }
};

int main(){

    //Thread with a function
    std::thread t1{thread_1};

    //Thread with a functor
    std::thread t2{ftor_thread()};

    //wait for both threads to finish
    t1.join();
    t2.join();

    return 0;
}