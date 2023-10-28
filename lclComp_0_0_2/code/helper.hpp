#ifndef HELPER_H_
#define HELPER_H_
#include <cmath>
#include <string>
#include <iostream>
#define headlineSTR "--------------------------------------------------------------------------"
#define spaceList 32
namespace CompileLCL{
	    #define headlineSTR "--------------------------------------------------------------------------"
void printHeadline(std::string s);
	int intlog(double base, double x);
	std::string numberTString(int base,long long number);
	long long stringTNumber(int base, std::string number);
	bool endsWith(std::string phrase,std::string endWit);
	unsigned long long replace(std::string what,std::string with,std::string *in);
	void printSpace(int spaces);
	void printList(std::string firstArg,std::string secondArg);
}
#endif
