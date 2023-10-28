#ifndef ICOBJ_HPP
#define ICOBJ_HPP
#include "../keywords.hpp"
#include "../nullable.hpp"
#include "tokenize.hpp"
#include "datatype.hpp"
namespace CompileLCL
{
	class Tokenize;
    class ICObj
	{
	protected:
		Datatype* dt;
		std::string name;
		std::string *annotations;
		int *attributes;
		int *accessParas;
		Tokenize *tk;
		bool isNULL = true;

	public:
		ICObj();
		ICObj(Tokenize *tk);
		void resolveDT();
		void resolveName();
		void resolveAttributes();
		void resolveTransfers();
		void parseClass();
		bool moveTillDT(bool breakOnBacketClose);
	};
	class Variable : public ICObj{
		public:
		Variable();
		Variable(Tokenize *tk);
	};
	class ICObjWCh : public ICObj
	{
	protected:
		std::vector<ICObj> children;

	public:
		ICObjWCh();
		ICObjWCh(Tokenize *tk);
		void parseChildren();
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