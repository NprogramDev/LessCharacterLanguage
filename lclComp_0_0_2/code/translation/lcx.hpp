#ifndef LCX_HPP_
#define LCX_HPP_
#include <string>
#include "../simplyfy/keywords.hpp"
#include "../helper.hpp"
namespace CompileLCL{
    namespace LCX{
        inline std::string m[Keywords::length];
        std::string translate(std::string lcx);
        void init(Keywords::l a,std::string b);
        void reg();
    }
}
#endif