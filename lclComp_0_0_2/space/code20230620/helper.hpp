#ifndef HELPER_H_
#define HELPER_H_
#include <math.h>
#include <string>
#include <iostream>
namespace CompileLCL{
	int intlog(double base, double x);
	std::string numberTString(int base,long long number);
	long long stringTNumber(int base, std::string number);
}
#endif
