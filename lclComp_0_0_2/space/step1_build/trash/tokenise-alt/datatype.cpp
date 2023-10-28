#include "datatype.hpp"
using namespace CompileLCL;
#define ERROR_MULTIDATATYPE_COUNT_NO_DATATYPE 2000003
#define ERROR_MULTIDATATYPE_PARSE_NO_DATATYPE 2000004

Datatype* Datatype::getDatatype(Tokenize* tk){
	int nMDT = testforMultitype(tk);
	std::cout << "DTC: " << nMDT << std::endl;
	Datatype* dt;
	if(nMDT != 1){
		dt = new MultiDatatype(tk,nMDT);
		dt->parse();
	}else{
		//std::cout << "Posit: " << tk->iterator << std::endl;
		if(tk->cc() == Keywords::l::context_string){
			std::cout << "DT Type: Complex" << std::endl;
			dt = new ComplexDatatype(tk);
			dt->parse();
		}else if(tk->isSimpleDT() || tk->isClasslikeDT()){
			std::cout << "DT Type: Simple" << std::endl;
			dt = new SimpleDatatype(tk);
			dt->parse();
		}
	}
	return dt;
}
bool Datatype::parse(){tk->info("Datatype::parse should not be used!");return false;}
Datatype::Datatype(Tokenize *tk){
	this->tk = tk;
	this->lastIndex = 0;
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

int Datatype::testforMultitype(Tokenize* tk){
	int startIterator = tk->iterator;
	if(tk->iterator != 0)(*tk)--;
	// If no brackets are set => It's always a one Datatype Object
	if(tk->cc() != Keywords::l::para_bracket_round_open)return 1;
	bool isContext = false;
	int attributeNumber = 1;
	(*tk)++;
	// Loop trough till 2 DTs are following on each other => Int abc() => Datatype 1 = int && abc = Datatype 2
	while(true){
		if(isContext){
			if(tk->cc() == Keywords::l::context_string)isContext = false;
		}else
		if(tk->cc() == Keywords::l::context_string){
			isContext = true;
		}else
		if(tk->isSimpleDT()){
			
		}else
		if(tk->cc() == Keywords::l::para_bracket_round_close){
			break;
		}else if(tk->cc() == Keywords::l::para_bracket_round_open){
			while(tk->cc() == Keywords::l::para_bracket_curly_close){
				(*tk)++;
			}
		}
		if(tk->cc() == Keywords::l::para_comma){
			attributeNumber++;
		}else{
			tk->err(ERROR_MULTIDATATYPE_COUNT_NO_DATATYPE, "Is not a Datatype <=> Exspected ')'");
			attributeNumber = -1;
			break;
		}
		(*tk)++;
	}
	tk->iterator = startIterator;
	return attributeNumber;
}
int Datatype::getLastIndex(){
	return lastIndex;
}
void Datatype::print(int death){	}
/**
 * Multi-Datatype
*/

MultiDatatype::MultiDatatype(Tokenize* tk,int numberOfRTDTs) : Datatype(tk){
	this->tk = tk;
	this->number = numberOfRTDTs;
	this->value = new Datatype*[numberOfRTDTs];
}
bool MultiDatatype::parse(){
	int startPos = tk->iterator;
	(*tk)--;
	if(tk->cc() != Keywords::l::para_bracket_round_open)return false;
	(*tk)++;
	int i = 0;
	while(tk->cc() != Keywords::l::para_bracket_round_close){
		if(tk->cc() == Keywords::l::para_bracket_round_open){(*tk++);}
		int n = testforMultitype(tk);
		if(n != 0){
			this->value[i] = new MultiDatatype(tk,n);
			this->value[i]->parse();
			tk->iterator = this->value[i]->getLastIndex();
			i++;
		}else
		if(tk->isSimpleDT()){
			this->value[i] = new SimpleDatatype(tk);
			this->value[i]->parse();
			tk->iterator = this->value[i]->getLastIndex();
			i++;
		}else
		if(tk->cc() == Keywords::l::context_string){
			this->value[i] = new ComplexDatatype(tk);
			this->value[i]->parse();
			tk->iterator = this->value[i]->getLastIndex();
			i++;
		}else{
			tk->err(ERROR_MULTIDATATYPE_PARSE_NO_DATATYPE,"Not a Datatype <=> Exspected a Datatype, like int, double, bool etc.");
			
			break;
		}
		(*tk)++;
	}
	return true;
}
void MultiDatatype::print(int death){
	for(int i = 0; i < number; i++){
		printSpace(death);
		std::cout << "Multitype: " << std::endl;
		Datatype d = *(value[i]);
		d.print(death + 1);
	}
}
/**
 * Complex-Datatype
*/
ComplexDatatype::ComplexDatatype(Tokenize* tk) : Datatype(tk){
}
bool ComplexDatatype::parse(){
	this->value = "";
	int startIt = tk->iterator;
	if(tk->cc() == Keywords::l::para_bracket_round_open){
		(*tk)++;
	}
	while(tk->cc() != Keywords::l::context_string){
		this->value +=(char) tk->cc();
		(*tk)++;
	}
	lastIndex = tk->iterator;
	tk->iterator = startIt;
	return true;
}
void ComplexDatatype::print(int death){
	printSpace(death);
	std::cout << this->value << std::endl;
}
/**
 * Simple-Datatype
*/
SimpleDatatype::SimpleDatatype(Tokenize* tk) : Datatype(tk){
}
bool SimpleDatatype::parse(){
	int startIt = tk->iterator;
	if(tk->cc() == Keywords::l::para_bracket_round_open){
		(*tk)++;
	}
	this->value = tk->cc();
	lastIndex = tk->iterator;
	tk->iterator = startIt;
	std::cout << "Parsed DT!" << std::endl;
	return true;
}
void SimpleDatatype::print(int death){
	printSpace(death);
	std::cout << Keywords::m[this->value] << std::endl;
}










/*//If it is a context String, then only execute the next part if the context ends
		if(tk->cc() == Keywords::l::context_string){
			isContext = !isContext;
			// If it is no context string any more => jump to the next (if Comma jump to the next) 
			if(!isContext){
				(*tk)++;
				if(tk->cc() == Keywords::para_comma){return attributeNumber;}
				(*tk)++;
				//if the next Keyword after the context is another context_string or datatype => n+1
				if(tk->isDT()){
					attributeNumber++;
					continue;
				}else if(tk->cc() == Keywords::context_string){
					attributeNumber++;
					continue;
				}
			}
		}else
		if(tk->isDT()){
			(*tk)++;
			if(tk->cc() == Keywords::para_comma){return attributeNumber;}
			(*tk)++;
			if(tk->isDT()){
				attributeNumber++;
				continue;
			}else if(tk->cc())
		}else*/