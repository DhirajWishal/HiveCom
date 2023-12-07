#include <iostream>

#include "Simulator/Reactor.hpp"

int main()
{
	HiveCom::Reactor reactor;
	reactor.execute([]
					{ 
						std::cout << "Welcome to HiveCom!" << std::endl;
						std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl; });
}
