#ifndef SIMPLY_H_
#define SIMPLY_H_
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include "keywords.hpp"
#include "../helper.hpp"
namespace CompileLCL
{
	class Simply
	{
	private:
		std::string text;
		std::string binary;
		int iterator;
		void parseAnyClass();
		static int attributesClass[];
		static int outerClass[5];

	public:
		Simply(std::string txt) : text(txt), binary(""), iterator(0) {}
		char cc(int offset);
		bool nextCharsAre(std::string str);
		bool isCommentChar();
		bool isVoidChar(char c);
		char cc() { return cc(0); }
		bool isNOversize();
		void jumpVoids();
		void jumpComment();
		void jumpUnused();
		bool isNumber();
		void processNumber();
		std::string getResult();
		void parse();
	};
}
#endif
