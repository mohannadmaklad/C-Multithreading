#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <fstream>

/*
The classic lazy-initialization case is where you have an object that is expensive to construct but isn't always needed.
*/

class LogFile{
public:
	LogFile(){
		//It's not always needed, so it needs to be moved to the "Log" function
		//_f.open("log.txt"); //Needs to be opened only one time
	};

	void Log(std::string msg){

#if 0	/* Too much over head */
		{ //Scoped to release open_mu
		std::lock_guard<std::mutex> open_guard(_open_mu);
		if(_f.is_open()){
			_f.open("log.txt");
		}
		}
#endif
		std::call_once(_once_flag, [&](){_f.open("log.txt");}); //Called once and is thread safe
		std::lock_guard<std::mutex> lock_guard(_write_mu);
		_f << msg << std::endl;
	}

private:
	std::ofstream _f;
	std::once_flag _once_flag;
	std::mutex _open_mu;
	std::mutex _write_mu;
};

int main(){
	std::cout<< "hello world";
}