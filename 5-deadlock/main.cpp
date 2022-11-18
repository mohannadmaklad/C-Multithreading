#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>


class LogFile{
	public:
	LogFile(){
		//_f.open("log.txt");
		std::cout << "constructor is called" << std::endl;
	}

	void Log(std::string str, int id){
		std::cout << "LOG: Acquiring mutex 1\n";
		std::lock_guard<std::mutex> guard(mu);
		std::cout << "LOG: Acquired mutex 1\n";

		std::cout << "LOG: Acquiring mutex 2\n";
		std::lock_guard<std::mutex> guard2(mu2);
		std::cout << "LOG: Acquired mutex 2\n";

		std::cout << str << " " << id << std::endl;
	}

	void DeadlockLog(std::string str, int id){
		std::cout << "ILOG: Acquiring mutex 2\n";
		std::lock_guard<std::mutex> guard(mu2);
		std::cout << "ILOG: Acquired mutex 2\n";

		std::cout << "ILOG: Acquiring mutex 1\n";
		std::lock_guard<std::mutex> guard2(mu);
		std::cout << "ILOG: Acquired mutex 1\n";

		std::cout << str << " " << id << std::endl;
	}

	//Solution #1
	//Lock and unlock in the same order
	void NoDeadlockLog_1(std::string str, int id){
		std::cout << "ILOG: Acquiring mutex 1\n";
		std::lock_guard<std::mutex> guard2(mu);
		std::cout << "ILOG: Acquired mutex 1\n";

		std::cout << "ILOG: Acquiring mutex 2\n";
		std::lock_guard<std::mutex> guard(mu2);
		std::cout << "ILOG: Acquired mutex 2\n";

		std::cout << str << " " << id << std::endl;
	}

	//Solution #2
	//use the standard std::lock with std::adopt_lock to unlock before going out of scope
	void NoDeadlockLog_2(std::string str, int id){
		std::lock(mu2,mu);
		std::lock_guard<std::mutex> guard(mu2, std::adopt_lock);
		std::lock_guard<std::mutex> guard2(mu, std::adopt_lock);

		std::cout << str << " " << id << std::endl;
	}

private:
	std::ofstream _f;
	std::mutex mu;
	std::mutex mu2;
};

void log_to_file(LogFile& f){
	for(auto i=100; i>0; --i){
		f.Log("logging from spawned thread", i);
	}
}


int main(){
	LogFile f;
	std::thread t{log_to_file,std::ref(f)};
	for(auto i=100; i>0; --i){
		f.NoDeadlockLog_2("logging from main thread" , i);
	}

	t.join();
}