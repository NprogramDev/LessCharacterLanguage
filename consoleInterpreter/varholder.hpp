#ifndef LCLS_VARHOLDER_HPP_
#define LCLS_VARHOLDER_HPP_
#include "string.hpp"
#include "map.hpp"
#include "var.hpp"
namespace LCLS{
	class VarHolder{
		Map<String,Var> vars;
	};
}
#endif
