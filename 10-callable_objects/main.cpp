#include <iostream>
#include <functional>
#include <thread>
#include <mutex>

class a{
public:
	void operator()(std::string msg){
		std::cout << msg << "from functor" << std::endl;
	}

	void print_member_func(std::string msg){
		std::cout << msg << "from member function" << std::endl;
	}
};

void print_val(int v){
	std::cout << "value is : " << v << std::endl;
}

int main(){
	auto binded_func = std::bind(print_val,5);
	auto binded_func2 = std::bind(a(),"hello there ");
	auto binded_func3 = std::bind(a(),"Should be called once ");

	binded_func();
	binded_func2();

	std::once_flag flag;
	for(auto i=0; i<10; i++){
		std::call_once(flag,binded_func3);
	}
	//pass a member function
	a A();
	std::thread t {&a::print_member_func,A,"hello there "};
}