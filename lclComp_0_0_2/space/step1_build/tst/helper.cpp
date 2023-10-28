#include "helper.hpp"
int CompileLCL::intlog(double base, double x){
	return (int)(log(x) / log(base));
}
std::string CompileLCL::numberTString(int base,long long number){
	std::string rt = "";
	int x = intlog(base,number);
	x = pow(base,x);
	while(x > 0){
		long long y = number / x;
		if(y >= 0){
			rt += y;
		}else{
			rt += (char)0;
		}
		number = number % x;
		x = x / base;
	}
	return rt;
}
