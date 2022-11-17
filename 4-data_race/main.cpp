#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex m;


void print_raw_mutex(std::string msg){
    for(auto i=0; i<100; i++){
        m.lock();
        std::cout << msg << std::endl;
        m.unlock();
    }
}

void print_lock_guard(std::string msg){
    std::lock_guard<std::mutex> guard(m);
        for(auto i=0; i<100; i++){
        std::cout << msg << std::endl;
    }
}

int main(){
    std::string main_msg = "Hello from main";
    std::string thread_msg = "Hello from thread";

    std::thread t{print_lock_guard,thread_msg};

    for(auto i=0; i<100; i++){
        std::cout << main_msg << std::endl;
    }

    t.join();
}