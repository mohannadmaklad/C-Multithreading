#include <iostream>
#include <thread>
#include <chrono>


class Functor{
    public:
    Functor() = default;

    // //Copy constructor
    // Functor(const Functor& other){
    //     std::cout << "Copy Constructor is called" << std::endl;
    // }
    //call operator
    void operator()(std::string& msg){
        std::cout << "spawned thread id " << std::this_thread::get_id() << std::endl;
        std::cout << msg << std::endl;
        msg = "New message";
    }
};

int main(){

    std::cout << "main thread id " << std::this_thread::get_id() << std::endl;
    std::cout << std::thread::hardware_concurrency() << std::endl;
    std::string message {"from the functor thread"};
    //thread constructor takes a fucntor by value, so std::ref is needed
    std::thread t{Functor(),std::ref(message)}; //message is shared bw both threads
    //std::thread t{Functor(),std::move(message)}; //better, can't use message later

    //std::thread t2 = t; //not allowed, t's = operator and copy constructors are both delted
    std::thread t2 = std::move(t);
    std::cout << "t2 thread id " << t2.get_id() << std::endl;
    t2.join();

    std::cout << message << std::endl;
    return 0;
}