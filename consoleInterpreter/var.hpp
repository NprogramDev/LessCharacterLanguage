#ifndef LCLS_VAR_HPP_
#define LCLS_VAR_HPP_
namespace LCLS{
	enum VarType{
		START_OF_ENUM,
		BOOLEAN, // 8 bit only true or false
		BYTE, // 8 bit
		CHAR, // 16 bit UTF-16
		SHORT, // 16 bit
		INT,   // 32 bit
		LONG, // 64 bit
		CLASS, // => Size attr
		END_OF_ENUM;
	};
	/* VarSize_t
	* The VarSize_t defines the maximum var size
	*/
	typedef unsigned int VarSize_t; 
	class Var{
		VarType t;
		
		void* val;	
	};
}
#endif
