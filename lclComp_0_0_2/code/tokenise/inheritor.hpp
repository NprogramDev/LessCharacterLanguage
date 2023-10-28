#ifndef INHERITOR_HPP_
#define INHERITOR_HPP_
#include "iterator.hpp"
#include "genericdt.hpp"
namespace CompileLCL{
    class inheritor{
        Iterator i;
        particle* pc;
        GenericDT* gdt;
        unsigned long long array = -1;
        public:
            inheritor(Iterator &i);
            unsigned int parse();
            void resolveArray();
            particle* getParticle();
    };
}

#endif