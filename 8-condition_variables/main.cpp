#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

std::queue<int> q;
std::mutex queue_mu;
std::mutex cout_mu;
std::condition_variable message_avilable;

void producer_thread(void){
	int count = 10;
	while(count != 0){
		std::unique_lock<std::mutex> ul(queue_mu);
		//with defer lock you have to lock/unlock the locker manually
		std::unique_lock<std::mutex> cout_locker(cout_mu, std::defer_lock);
		q.push(count);
		ul.unlock();
		
		{
			cout_locker.lock();
			std::cout << "Producing " << count << std::endl;
			message_avilable.notify_one();
			cout_locker.unlock();
		}
		//Producing data every 100 ms
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		--count;
	}
}

//Too much blocking time
void blocking_consumer_thread(void){
	int data = 0;

	while(data != 1){
		std::unique_lock<std::mutex> ul(queue_mu);
		//with defer lock you have to lock/unlock the locker manually
		std::unique_lock<std::mutex> cout_locker(cout_mu, std::defer_lock);

		if(!q.empty()){
			data = q.front();
			q.pop();
			ul.unlock();
		}
		else{
			ul.unlock();
			cout_locker.lock();
			std::cout << "Couldn't find any data to process\n";
			cout_locker.unlock();
		}

		{
			cout_locker.lock();
			std::cout << "processing " << data << std::endl;
		}
	}
}



//Solution #1: Wake only after being notified
void event_driven_consumer_thread(void){
	int data = 0;

	while(data != 1){
		std::unique_lock<std::mutex> queue_locker(queue_mu);
		//with defer lock you have to lock/unlock the locker manually
		std::unique_lock<std::mutex> cout_locker(cout_mu, std::defer_lock);

		//wait will unlock the queue locker till a new message is available
		//message_avilable.wait(queue_locker); //Spurious wake

		message_avilable.wait(queue_locker, []()->bool{return !q.empty();}); //wake only if the queue is not empty
		data = q.front();
		q.pop();
		queue_locker.unlock();
		

		{
			cout_locker.lock();
			std::cout << "processing " << data << std::endl;
		}
	}
}

//Solution #2: Synchronize both threads in time triggered manner
void time_triggered_consumer_thread(void){
	int data = 0;

	while(data != 1){
		std::unique_lock<std::mutex> ul(queue_mu);
		//with defer lock you have to lock/unlock the locker manually
		std::unique_lock<std::mutex> cout_locker(cout_mu, std::defer_lock);

		if(!q.empty()){
			data = q.front();
			q.pop();
			ul.unlock();
			//Sleep for the same amount of time
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else{
			ul.unlock();
			cout_locker.lock();
			std::cout << "Couldn't find any data to process\n";
			cout_locker.unlock();
		}

		{
			cout_locker.lock();
			std::cout << "processing " << data << std::endl;
		}
	}
}

int main(){
	std::thread producer {producer_thread};
	std::thread consumer {time_triggered_consumer_thread};
	producer.join();
	consumer.join();
	std::cout<< "hello world";
}