#include "tokenize.hpp"
using namespace CompileLCL;
Tokenize::Tokenize(std::string &binary)
{
	this->binary = binary;
}
bool Tokenize::IsDTWCh()
{
	return this->isClasslikeDT();
}
unsigned int Tokenize::cc() { return binary.at(iterator); }
bool Tokenize::isSimpleDT()
{
	return cc() == Keywords::l::para_int ||
		   cc() == Keywords::l::para_boolean ||
		   cc() == Keywords::l::para_char ||
		   cc() == Keywords::l::para_string ||
		   cc() == Keywords::l::para_byte ||
		   cc() == Keywords::l::para_float ||
		   cc() == Keywords::l::para_long ||
		   cc() == Keywords::l::para_double ||
		   cc() == Keywords::l::para_short ||
		   cc() == Keywords::l::para_object ||
		   cc() == Keywords::l::para_void ||
		   cc() == Keywords::l::para_ab_obj_cl;
}
NULLABLE Tokenize::operator++(int x)
{
	this->iterator++;
	return NULLOBJ;
}
NULLABLE Tokenize::operator--(int x)
{
	this->iterator--;
	return NULLOBJ;
}
bool Tokenize::isClasslikeDT()
{
	return cc() == Keywords::l::para_class ||
		   cc() == Keywords::l::para_enum ||
		   cc() == Keywords::l::para_record ||
		   cc() == Keywords::l::para_interface;
}
bool Tokenize::isDT()
{
	return cc() == Keywords::l::context_string || isSimpleDT() || isClasslikeDT();
}
bool Tokenize::isOversize()
{
	return this->iterator >= this->binary.length();
}
void Tokenize::err(int exitCode, std::string codeX)
{
	std::cout << "ERROR: " + std::to_string(exitCode) + " caused through: " + codeX;
}
void Tokenize::info(std::string msg)
{
	std::cout << "Tokenize: " + msg;
}
bool Tokenize::isAttribute()
{
	return cc() == Keywords::l::para_public ||
		   cc() == Keywords::l::para_private ||
		   cc() == Keywords::l::para_protected ||
		   cc() == Keywords::l::para_static ||
		   cc() == Keywords::l::para_default ||
		   cc() == Keywords::l::para_abstract ||
		   cc() == Keywords::l::para_synchronized ||
		   cc() == Keywords::l::para_final ||
		   cc() == Keywords::l::para_transient ||
		   cc() == Keywords::l::para_volatile ||
		   cc() == Keywords::l::para_strictfp ||
		   cc() == Keywords::l::para_goto_allowance ||
		   cc() == Keywords::l::para_assert;
}
