#include "helper.hpp"
int CompileLCL::intlog(double base, double x)
{
	return (int)(log(x) / log(base));
}
std::string CompileLCL::numberTString(int base, long long number)
{
	std::string rt = "";
	//	std::cout << "srcX " << number << std::endl;
	if (number < 0)
	{
		number *= -1;
	}
	long long x = intlog(base, number);
	//	std::cout << "logX " << x << std::endl;
	x = pow(base, x);
	//	std::cout << "powX " << x << std::endl;
	while (x > 0)
	{
		long long y = number / x;
		if (y >= 0)
		{
			rt += (char)y;
		}
		else
		{
			rt += (char)0;
		}
		number = number % x;
		x = x / base;
	}
	return rt;
}
long long CompileLCL::stringTNumber(int base, std::string number)
{
	long long x = 1;
	long long result = 0;
	for (int i = number.length() - 1; i > -1; i--)
	{
		result += number.at(i) * x;
		x *= base;
	}
	return result;
}
void CompileLCL::printSpace(int spaces)
{
	for (int i = 0; i < spaces; i++)
	{
		std::cout << " ";
	}
}
void CompileLCL::printList(std::string firstArg, std::string secondArg)
{
	for (int i = 0; i < spaceList; i++)
	{
		if (i < firstArg.length())
		{
			std::cout << firstArg[i];
		}
		else
		{
			std::cout << " ";
		}
	}
	std::cout << "|";
	std::cout << secondArg << std::endl;
}
void CompileLCL::printHeadline(std::string s)
{
	std::string hds = headlineSTR;
	std::cout << hds << std::endl;
	int start = (hds.length() / 2) - (s.length() / 2);
	for (int i = 0; i < hds.length(); i++)
	{
		if (i == start)
		{
			std::cout << s;
			i += s.length();
		}
		std::cout << " ";
	}
	std::cout << std::endl
			  << hds << std::endl;
}
bool CompileLCL::endsWith(std::string phrase, std::string endWit)
{
	if (phrase.length() < endWit.length())
		return false;
	for (size_t i = 1; i <= endWit.length(); i++)
	{
		if (phrase.at(phrase.length() - i) != endWit.at(endWit.length() - i))
			return false;
	}
	return true;
}
using namespace CompileLCL;
unsigned long long CompileLCL::replace(std::string what,std::string with,std::string *in){
	return 0;
}