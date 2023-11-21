#ifndef LCLS_STRING_HPP_
#define LCLS_STRING_HPP_
#include <string>

namespace LCLS{
	class String : public std::string{
public:
		String();
		String(const std::string& str);
		String(const std::string& str, size_t pos, size_t len);
		String(const char* s);
		String(const char* s, size_t n);
		String(size_t n, char c);
	};
}

#endif
