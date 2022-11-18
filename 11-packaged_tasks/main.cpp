#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<std::packaged_task<int()>> q;
std::queue<std::future<int>> results;
std::mutex queue_mutex;
std::condition_variable new_message;

int sum(int a, int b){
	return a+b;
}

void producer(){
	for(auto i=0; i<10; i++){
		std::packaged_task<int()> task{std::bind(sum,i,i)};
		std::unique_lock<std::mutex> locker(queue_mutex);
		
		//push the future into the results queue
		results.push(task.get_future());

		q.push(std::move(task));
		locker.unlock();


		//notify worker threads to do the task
		new_message.notify_one();
	}
}

void worker(){
	int count {0};
	while(count < 10){
		std::unique_lock<std::mutex> queue_unique_locker(queue_mutex);
		new_message.wait(queue_unique_locker,[]()->bool{return !q.empty();});
		auto t = std::move(q.front()); //std::move is necessary, packaged_tasks cannot be copied
		q.pop();
		queue_unique_locker.unlock();
		t();

		count++;
	}
}


int main(){
	//Basic packaged task
	std::packaged_task<int(int,int)> task_1{sum};
	task_1(2,3);
	std::cout << task_1.get_future().get() << std::endl; //Can Run in a different context

	//Bound packaged task 
	std::packaged_task<int()> task_2{std::bind(sum,8,9)};	
	task_2();
	std::cout << task_2.get_future().get() << std::endl;

	//a packaged task links a callable object with a future

	//packaged tasks cannot be copied
	//std::packaged_task<int()> task_3 = task_2;

	//Worker and producer threads
	std::thread producer_thread {producer};
	std::thread worker_thread {worker};

	producer_thread.join();
	worker_thread.join();
	

	int i = 0;
	//print out the results
	while(!results.empty()){
		std::cout << "getting the results of " << i << std::endl;
		auto res_fu = std::move(results.front());
		auto res = res_fu.get();
		results.pop();

		std::cout << "result " << i++ << " : " << res << std::endl;
	}
}