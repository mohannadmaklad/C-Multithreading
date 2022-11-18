#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>


std::mutex mu;
std::mutex mu2;
std::mutex mu3;

void thread_1(){
	for(auto i=0; i<100; i++){
		std::lock_guard<std::mutex> locker(mu);
		std::unique_lock<std::mutex> ulock_1(mu2);
		//defer lock doesn't lock the mutex
		std::unique_lock<std::mutex> ulock_2(mu3, std::defer_lock);
		
		if (ulock_1.owns_lock()){
			std::cout << "mu2 is locked\n";
		} else {
			std::cout << "mu2 is not locked\n";
		}

		if (ulock_2.owns_lock()){
			std::cout << "mu3 is locked\n";
		} else {
			std::cout << "mu3 is not locked\n";
			//You can lock and unlock multiple times
			ulock_2.lock();
			ulock_2.unlock();
			ulock_2.lock();
			ulock_2.unlock();
			ulock_2.lock();
			ulock_2.unlock();
		}

		std::cout << "In " << "thread 1\n";
	}
}

int main(){
	std::thread t1{thread_1};

	for(auto i=0; i<100; i++){
		std::cout << "In " << "main\n";
	}

	t1.join();
	
}