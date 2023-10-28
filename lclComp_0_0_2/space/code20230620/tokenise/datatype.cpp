#include "datatype.hpp"
using namespace CompileLCL;
Datatype::Datatype(Tokenize *tk){
	this->tk = tk;
	/*this->isComplexDT = !tk->isSimpleDT();
	complexDT = "";
	if (this->isComplexDT)
	{
		(*tk)++;
		while (tk->cc() != Keywords::l::context_string)
		{
			complexDT += (char)tk->cc();
			(*tk)++;
		}
		tk->info("Found Complex Datatype: " + this->complexDT);
	}
	else
	{
		this->simpelDT = tk->cc();
		tk->info("Found Simple Datatype: " + Keywords::m[this->simpelDT]);
	}*/
}
void Datatype::parse(){}
int Datatype::testforMultitype(Tokenize* tk){
	int startIterator = tk->iterator;
	bool isContext = 0;
	int attributeNumber = 1;
	// Loop trough till 2 DTs are following on each other => Int abc() => Datatype 1 = int && abc = Datatype 2
	while(1){
		//If it is a context String, then only execute the next part if the context ends
		if(tk->cc() == Keywords::l::context_string){
			isContext = !isContext;
			if(!isContext){
				(*tk)++;
				//if the next Keyword after the context is 2
				if(tk->isDT()){
					break;
				}
			}
		}else
		if(tk->isDT()){
			(*tk)++;
			if(tk->isDT()){
				break;
			}
		}else
		if(tk->cc() == Keywords::l::para_comma){
			attributeNumber++;
		}else{
			tk->err(2000003, "Is not a Datatype");
		}
	}
	tk->iterator = startIterator;
	return attributeNumber;
}
Datatype* Datatype::getDatetype(Tokenize* tk){
	int nMDT = testforMultitype(tk);
	Datatype* dt;
	if(nMDT != 1){
		dt = new MultiDatatype(tk,nMDT);
		dt->parse();
	}else{
		if(tk->cc() == Keywords::l::context_string){
			dt = new ComplexDatatype(tk);
			dt->parse();
		}else if(tk->isSimpleDT()){
			dt = new SimpleDatatype(tk);
			dt->parse();
		}
	}
	return dt;
}
/**
 * Multi-Datatype
*/

MultiDatatype::MultiDatatype(Tokenize* tk,int numberOfRTDTs) : Datatype(tk){
}
void MultiDatatype::parse(){
	
}
/**
 * Complex-Datatype
*/
ComplexDatatype::ComplexDatatype(Tokenize* tk) : Datatype(tk){
}
void ComplexDatatype::parse(){
	this->value = "";
	while(tk->cc() != Keywords::l::context_string){
		this->value +=(char) tk->cc();
	}
}
/**
 * Simple-Datatype
*/
SimpleDatatype::SimpleDatatype(Tokenize* tk) : Datatype(tk){
}
void SimpleDatatype::parse(){
	this->value = tk->cc();
}