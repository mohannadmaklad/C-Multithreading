#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int calculate_sum(int a, int b){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return a+b;
}

int calculate_sum_2(int a, std::future<int>& fut){
	int sum = a + fut.get();
	std::cout << "Sum is ready, promise is kept!\n";
	return sum;
}


int main(){
	
	//std::future<int> fu = std::async(std::launch::async,calculate_sum,5,6); //creates a new thread
	//std::future<int> fu = std::async(std::launch::deferred,calculate_sum,5,6); //wait till get is called and run in the same thread
	std::future<int> fu = std::async(std::launch::deferred | std::launch::async ,calculate_sum,5,6); //creating a thread is determined by the implementation, the default
	int sum = fu.get();

	std::promise<int> p;
	std::future<int> fu2 = p.get_future();

	//You can use a shared future to pass it to multiple threads, as it can be copied
	std::shared_future<int> sh = fu2.share();
	
	std::future<int> fu3 = std::async(std::launch::async, calculate_sum_2, 4, std::ref(fu2));

	p.set_value(3); // if you can keep your promise
	//p.set_exception(std::make_exception_ptr(std::runtime_error("Can't keep promise!")));
	std::cout << "main thread sleeping\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << "main thread woke up!\n";

	std::cout << "Result 1 : " << sum << std::endl;
	std::cout << "Result 2 : " << fu3.get() << std::endl;
}