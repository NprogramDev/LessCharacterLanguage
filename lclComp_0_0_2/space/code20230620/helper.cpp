#include "helper.hpp"
int CompileLCL::intlog(double base, double x){
	return (int)(log(x) / log(base));
}
std::string CompileLCL::numberTString(int base,long long number){
	std::string rt = "";
//	std::cout << "srcX " << number << std::endl;
	long long x = intlog(base,number);
//	std::cout << "logX " << x << std::endl;
	x = pow(base,x);
//	std::cout << "powX " << x << std::endl;
	while(x > 0){
		long long y = number / x;
		if(y >= 0){
			rt += (char )y;
		}else{
			rt += (char)0;
		}
		number = number % x;
		x = x / base;
	}
	return rt;
}
long long CompileLCL::stringTNumber(int base, std::string number){
	long long x = 1;
	long long result = 0;
	for(int i = number.length()-1; i > -1; i--){
		result += number.at(i) * x;
		x *= base;
	}
	return result;
}
