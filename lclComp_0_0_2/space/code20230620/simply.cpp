#include "simply.hpp"

using namespace CompileLCL;
char Simply::cc(int offset)
{
	return text.at(iterator + offset);
}
bool Simply::nextCharsAre(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (cc(i) != str.at(i))
		{ /*std::cout << "Return False! "<< str.at(i) << "|" << cc(i) << std::endl;*/
			return 0;
		}
	}
	//		std::cout << "Return True" << std::endl;
	return 1;
}
bool Simply::isCommentChar()
{
	return nextCharsAre(Keywords::m[Keywords::l::para_comment_area_start]) || nextCharsAre(Keywords::m[Keywords::l::para_comment_line]);
}
bool Simply::isVoidChar(char c)
{
	return c == ' ' || c == '\n' || c == '\t';
}
bool Simply::isNOversize()
{
	return iterator < text.length() && iterator >= 0;
}
void Simply::jumpVoids()
{
	while (isVoidChar(cc()) && isNOversize())
	{
		iterator++;
	}
}
void Simply::jumpComment()
{
	if (isCommentChar())
	{
		iterator++;
		if (isNOversize())
		{ /*END*/
			return;
		}
		bool isArea = nextCharsAre(Keywords::m[Keywords::l::para_comment_area_start]);
		iterator++;
		while (isNOversize())
		{
			if (!isArea && cc() == '\n')
			{
				iterator++;
				break;
			}
			if (isArea)
			{
				if (nextCharsAre(Keywords::m[Keywords::l::para_comment_area_end]))
				{
					iterator++;
					break;
				}
			}
			iterator++;
		}
	}
}
void Simply::jumpUnused()
{
	while (isVoidChar(cc()) || isCommentChar())
	{
		jumpVoids();
		jumpComment();
	}
}
bool Simply::isNumber()
{
	return ((cc() == '.' && (cc(1) == '1' || cc(1) == '2' || cc(1) == '3' || cc(1) == '4' || cc(1) == '5' || cc(1) == '6' || cc(1) == '7' || cc(1) == '8' || cc(1) == '9' || cc(1) == '0')) || cc() == '0' || cc() == '1' || cc() == '2' || cc() == '3' || cc() == '4' || cc() == '5' || cc() == '6' || cc() == '7' || cc() == '8' || cc() == '9');
}
void Simply::processNumber()
{
	binary += Keywords::l::number;
	std::string number = "";
	bool hasPoint = 0;
	while (isNumber() || cc() == 'b' || cc() == 'x' || cc() == 'o')
	{
		number += cc();
		if (cc() == '.')
		{
			hasPoint = 1;
		}
		iterator++;
	}
	int it = number.length() - 1;
	long long num = 0;
	unsigned char sign = 0;
	if (hasPoint)
	{
		double x = std::stof(number);
		if (x < 0)
		{
			sign = Keywords::l::negativ_float;
		}
		else
		{
			sign = Keywords::l::positiv_float;
		}
		long long *p = (long long *)&x;
		num = *p;
	}
	else
	{
		num = std::stoi(number);
		if (num < 0)
		{
			sign = Keywords::l::negativ_int;
		}
		else
		{
			sign = Keywords::l::positiv_int;
		}
	}
	//	std::cout << (int) sign <<std::endl;
	//	binary += Keywords::l::number;
	binary += sign;
	std::string s = numberTString(128, num);
	for (int i = 0; i < s.length(); i++)
	{
		unsigned char x = s.at(i);
		x += Keywords::l::number + 1;
		binary += x;
	}
	binary += Keywords::l::number;
	// TODO NUMBER
}
void Simply::parse()
{
	binary = "";
	bool isContextString = 0;
	bool isActualString = 0;
	while (isNOversize())
	{
		if (!isActualString)
		{
			jumpUnused();
		}
		int lastLength = 0, lastIndex = 0;
		// search for longest fitting keyword
		for (int i = Keywords::l::start_of_enum + 1; i < Keywords::end_of_enum; i++)
		{
			if (nextCharsAre(Keywords::m[i]))
			{
				//					std::cout << "SM: " << i << " | " << Keywords::m[i] << std::endl;
				//					std::cout << "Length: " << lastLength << "|" << Keywords::m[i].length() << std::endl;

				if (Keywords::m[i].length() > lastLength)
				{
					lastLength = Keywords::m[i].length();
					//						std::cout << "Set Last length to " << lastLength << std::endl;
					lastIndex = i;
				}
			}
		}
		// if a keywords fits =>run it
		if (lastLength != 0 && !isActualString)
		{
			if (isContextString)
			{
				isContextString = 0;
				binary += Keywords::l::context_string;
				//					std::cout << "Add cs end to binary" << std::endl;
			}
			if (lastIndex == Keywords::l::para_quotes)
			{
				isActualString = 1;
				binary += Keywords::l::actual_string;
				iterator++;
				continue;
			}
			char x = lastIndex;
			binary += x;
			iterator += lastLength;
			//				isContextString = 0;
			// std::cout << "KW: " << Keywords::m[lastIndex] << " | " << lastIndex << std::endl;
		}
		else
		{
			if (isNumber())
			{
				processNumber();
				continue;
			}
			if (!isContextString && !isActualString)
			{
				isContextString = 1;
				binary += Keywords::l::context_string;
				//	std::cout << "Add cs to binary" << std::endl;
			}
			if (cc() == '#')
			{
				iterator++;
				continue;
			}
			if (cc() == Keywords::m[Keywords::l::para_quotes].at(0) && cc(-1) != Keywords::m[Keywords::l::para_escape_uppercase].at(0))
			{
				isActualString = 0;
				binary += Keywords::l::actual_string;
				iterator++;
				continue;
			}
			binary += cc();
			// std::cout << "Add cc to binary" << std::endl;
			iterator++;
		}
	}
}
std::string Simply::getResult()
{
	return binary;
}
