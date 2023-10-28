#include "icobj.hpp"
#include "../keywords.hpp"
using namespace CompileLCL;
// unsigned int Tokenize::outerKeywords[3] = {Keywords::l::para_class,Keywords::l::para_enum,Keywords::l::para_record};
// unsigned int Tokenize::datatypeKeywords[3] = {Keywords::l::para_int,Keywords::l::para_long,Keywords::l::para_byte};

ICObj::ICObj(Tokenize *tk){this->tk = tk;this->isNULL = false;}

// Move till Data Type
bool ICObj::moveTillDT(bool breakOnBracketClose)
{
	while (!tk->isDT())
	{
		tk->iterator++;
		if (tk->isOversize() || (tk->cc() == Keywords::l::para_bracket_curly_close && breakOnBracketClose))
		{
			return false;
		}
	}
	return true;
}

void ICObj::print(int depth){
	printSpace(depth);
	std::cout << "Something with Children: " << std::endl;
	printSpace(depth);
	std::cout << " Name: " << this->name << std::endl;
	std::cout << " Datatype:" << std::endl;
	this->dt->print(depth + 2);
	printSpace(depth +1);
	std::cout << "Attributes: " << std::endl;
	for(int i = 0; i < this->attriLength; i++){
		printSpace(depth + 2);
		std::cout << Keywords::m[this->attributes[i]] << std::endl;
	}
	printSpace(depth+1);
	std::cout << "Annotations: " << std::endl;
	for(int i = 0; i < this->annoLength; i++){
		printSpace(depth + 2);
		std::cout << this->annotations[i] << std::endl;
	}
}

ICObj::ICObj() {}
int ICObj::resolveDT()
{
	this->dt = Datatype::getDatatype(this->tk);
	return this->dt->getLastIndex();
}
void ICObj::resolveName()
{

	if (tk->cc() != Keywords::l::context_string)
	{
		tk->err(20000001, "Methodname is not a context string!");
		return;
	}
	this->name = "";
	//tk->info("Iteratorpos: " + std::to_string(tk->iterator));
	(*tk)++;
	int exe = 0;
	while (tk->cc() != Keywords::l::context_string)
	{
		this->name += (char)tk->cc();
		exe++;
		(*tk)++;
	}
	tk->info("Found name: " + this->name + "  -----" + std::to_string(exe));
}
void ICObj::resolveAttributes(){
	std::vector<int> attribu;
	while (tk->iterator >= 0)
	{
		if (!tk->isAttribute()){break;}
		//IF Access Parameter send to => accessPara
		//TODO Write accessPara ADDITION
		attribu.push_back(tk->cc());
		(*tk)--;
	}
	this->attributes = &attribu[0];
	this->attriLength = attribu.size();
	bool isContextStr = false;
	std::vector<std::string> annot;
	std::string str = "";
	while(tk->iterator >= 0){
		if(tk->cc() == Keywords::l::context_string && isContextStr){
			isContextStr = false;
			(*tk)--;
			if(tk->iterator < 0){break;}
			if(tk->cc() != Keywords::l::para_annotation){break;}
			//Reverse string, because backwards iteration
			std::reverse(str.begin(),str.end());
			annot.push_back(str);
			str = "";
			continue;
		}
		// 1. If there is no context string before the access Parameters => end this
		if(tk->cc() != Keywords::l::context_string && !isContextStr){
			break;
		//2. If there is, start the string.
		}else isContextStr = true;
		//3. Go one back to the next char
		(*tk)--;
		//4. if you are out of the content  => end this
		if(tk->iterator < 0){break;}
		// 5. Add the character to str
		str += (char) tk->cc();
	}
	this->annotations = &annot[0];
	this->annoLength = annot.size();
}
//resolves either the variables given to a method, or the extends / implements of a method
void ICObj::resolveTransfers(){
(*this->tk)++;


}
void ICObj::parseClass()
{
	//store datatype position
	int dtIndex = tk->iterator;
	//create Datatype Object store it in this->dt
	int lastDTPos = resolveDT();
	this->dt->print(0);
	// go one back, where the attributes should be
	//tk->iterator = dtIndex;
	if(tk->iterator != 0)(*tk)--;
	if(tk->cc() == Keywords::para_bracket_round_open)(*tk)--;
	// move back till there is no Attribute (including Annotations)
	resolveAttributes();
	//move back to Datatype
	tk->iterator = lastDTPos;
	// Resolve the name
	(*tk)++;
	resolveName();
	std::cout << "Name: " << this->name << std::endl;
	(*tk)++;
	if(tk->cc() == Keywords::l::para_bracket_round_open)resolveTransfers();
}
void ICObjWCh::parseClass(){
	this->ICObj::parseClass();
	(*tk)++;
	if(tk->cc() == Keywords::l::para_bracket_curly_open){
		this->parseChildren();
	}
}
#define DTSTATE_CLASS 0
#define DTSTATE_METHOD 1
#define DTSTATE_VARIABLE 2
void ICObjWCh::parseChildren()
{
	// move iterator to next datatype
	while (this->moveTillDT(true) )
	{
		std::cout << "Found DT at Position: " << this->tk->iterator << std::endl;
		// If this datatype has children use the ICObjWCh else the ICObj => parse it => store it in children
		ICObj obj = this->tk->IsDTWCh() ? (ICObj) ICObjWCh(this->tk) : (ICObj) Variable(this->tk).addTerminateKW(Keywords::l::para_semicolon);
		obj.parseClass();
		this->children.push_back(obj);
	}
}
void ICObjWCh::print(int depth){
	printSpace(depth +1);
	std::cout << "Children:" << std::endl;
	for(int i = 0; i < this->children.size();i++){
		this->children.at(i).print(depth+2);
	}
}
// goto ICObjWCh
void ROOT::parseClass()
{
	this->parseChildren();
}
void ROOT::print(){
	std::cout << "Child Count: " << this->children.size() << std::endl;
	for(int i = 0; i < this->children.size(); i++){
		this->children.at(i).print(0);
	}
}
Variable Variable::addTerminateKW(int kw){
	terminateKWs.push_back(kw);
	return *this;
}
void Variable::parseClass(){
	this->ICObj::parseClass();
	if(tk->cc() == Keywords::l::para_set){
		
	}else if(tk->cc() != Keywords::l::para_semicolon){
		tk->err(ERROR_SEMICOLON_NOT_FOUND_AT_VARIABLE,"Error, no terminate character found for the variable");
	}

}
void Variable::print(int depth){
	printSpace(depth);
	std::cout << "Variable: ["; 
	this->dt->print(0);
	std::cout  <<"," << this->name <<"," << this->defValue <<",]" << std::endl;
}
/**
 * Super matching Contrustors
*/

ICObjWCh::ICObjWCh(Tokenize *tk) : ICObj(tk)
{
}
ICObjWCh::ICObjWCh() {}
ROOT::ROOT(){};
ROOT::ROOT(Tokenize *tk) : ICObjWCh(tk)
{
}
Variable::Variable(): ICObj(){}
Variable::Variable(Tokenize *tk): ICObj(tk){}
