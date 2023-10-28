#ifndef TOKENIZE_H_
#define TOKENIZE_H_
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../nullable.hpp"
#include "../keywords.hpp"
// not Included => Looped Ref.
//#include "icobj.hpp"
namespace CompileLCL
{ /*
class ICObj;
class ICObjWCh : public ICObj;
class ROOT : public  ICObjWCh;*/
	//class Datatype;
	class Tokenize;	
	class Tokenize : public NULLABLE
	{
	private:
		//	static unsigned int datatypeKeywords[3];
		//	static unsigned int outerKeywords[3];
	public:
		std::string binary;
		int iterator = 0;
		Tokenize(std::string &binary);
		NULLABLE operator++(int x);
		NULLABLE operator--(int x);
		unsigned int cc();
		bool isSimpleDT();
		bool isClasslikeDT();
		bool isDT();
		bool IsDTWCh();
		bool isOversize();
		bool isAttribute();
		void err(int exitCode, std::string codeX);
		void info(std::string msg);
	};

	// Inner Class Object with Children

}
#endif
