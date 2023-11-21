#ifndef LCLS_INTERPRETER_HPP_
#define LCLS_INTERPRETER_HPP_
#include "string.hpp"
#include "var.hpp"
#include "dtholder.hpp"
#include "varholder.hpp"

namespace LCLS{
	class Interpreter{
	private:
		VarHolder vh;
		DTHolder dh;
	public:
		Interpreter();
		Var& eval(String& str);
	};
}
#endif
