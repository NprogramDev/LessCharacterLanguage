
#include "icobj.hpp"
#include "../keywords.hpp"
using namespace CompileLCL;
// unsigned int Tokenize::outerKeywords[3] = {Keywords::l::para_class,Keywords::l::para_enum,Keywords::l::para_record};
// unsigned int Tokenize::datatypeKeywords[3] = {Keywords::l::para_int,Keywords::l::para_long,Keywords::l::para_byte};

ICObj::ICObj(Tokenize *tk)
{
	this->tk = tk;
	this->isNULL = false;
}

// Move till Data Type
bool ICObj::moveTillDT(bool breakOnBracketClose)
{
	while (tk->isDT())
	{
		tk->iterator++;
		if (tk->isOversize() || (tk->cc() == Keywords::l::para_bracket_curly_close && breakOnBracketClose))
		{
			return 0;
		}
	}
	return 1;
}
ICObj::ICObj() {}
void ICObj::resolveDT()
{
	this->dt = new Datatype(this->tk);
}
void ICObj::resolveName()
{
	if (tk->cc() != Keywords::l::context_string)
	{
		tk->err(20000001, "Methodname is not a context string!");
		return;
	}
	this->name = "";
	(*tk)++;
	while (tk->cc() != Keywords::l::context_string)
	{
		this->name += (char)tk->cc();
		(*tk)++;
	}
	tk->info("Found name: " + this->name);
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
}
//resolves either the variables given to a method, or the extends / implements of a method
void ICObj::resolveTransfers(){
(*this->tk)++;

}
void ICObj::parseClass()
{
	int dtIndex = tk->iterator;
	resolveDT();
	(*tk)--;
	// move back till there is no Attribute (including Annotations)
	resolveAttributes();
	//move back to Datatype
	tk->iterator = dtIndex;
	(*tk)++;
	// Resolve the name
	resolveName();
}
ICObjWCh::ICObjWCh(Tokenize *tk) : ICObj(tk)
{
}
ICObjWCh::ICObjWCh() {}
void ICObjWCh::parseChildren()
{
	while (!this->moveTillDT(true))
	{
		ICObj obj = this->tk->IsDTWCh() ? ICObjWCh(this->tk) : ICObj(this->tk);
		obj.parseClass();
		this->children.push_back(obj);
	}
}
ROOT::ROOT(){};
ROOT::ROOT(Tokenize *tk) : ICObjWCh(tk)
{
}
void ROOT::parseClass()
{
	this->parseChildren();
}
void ROOT::print(){

}