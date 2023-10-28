#ifndef ICOBJ_HPP
#define ICOBJ_HPP
#include "../keywords.hpp"
#include "../nullable.hpp"
#include "tokenize.hpp"
#include "datatype.hpp"
#define ERROR_SEMICOLON_NOT_FOUND_AT_VARIABLE 20000002
namespace CompileLCL
{
	class Tokenize;
    class ICObj
	{
	protected:
		Datatype* dt;
		std::string name;
		std::string *annotations;
		int annoLength;
		int *attributes;
		int attriLength;
		int *accessParas;
		Tokenize *tk;
		bool isNULL = true;

	public:
		ICObj();
		ICObj(Tokenize *tk);
		int resolveDT();
		void resolveName();
		void resolveAttributes();
		void resolveTransfers();
		void parseClass();
		bool moveTillDT(bool breakOnBacketClose);
		void print(int depth);
	};
	class Variable : public ICObj{
		private:
		std::vector<int> terminateKWs;
		std::string defValue;
		public:
		Variable();
		Variable(Tokenize *tk);
		Variable addTerminateKW(int kw);
		void parseClass();
		void print(int depth);
	};
	class ICObjWCh : public ICObj
	{
	protected:
		std::vector<ICObj> children;

	public:
		ICObjWCh();
		ICObjWCh(Tokenize *tk);
		void parseChildren();
		void parseClass();
		void print(int depth);
	};
	class ROOT : public ICObjWCh
	{
	public:
		ROOT();
		ROOT(Tokenize *tk);
		void parseClass();
		void print();
	};
} // namespace CompileLCL


#endif