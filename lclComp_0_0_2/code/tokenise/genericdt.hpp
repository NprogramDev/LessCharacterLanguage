#ifndef GENERIC_DT_HPP_
#define GENERIC_DT_HPP_
#include "iterator.hpp"
namespace CompileLCL{
class GenericDT{
    private:
    Iterator i;
    public:
    GenericDT(Iterator &i);
    unsigned int parse();
};
}
#endif