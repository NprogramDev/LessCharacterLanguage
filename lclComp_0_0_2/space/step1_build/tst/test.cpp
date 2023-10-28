#include <iostream>
#include <string>
#include "helper.hpp"
int main(int argc, char** argv){
	std::string s = CompileLCL::numberTString(128,123456);
	for(int i = 0; i < s.length(); i++){
		int x = s.at(i);
		std::cout << x << "|";
	}
	std::cout << std::endl;
	return 0;
}
