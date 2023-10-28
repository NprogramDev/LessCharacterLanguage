#include "prebuilder.hpp"
using namespace CompileLCL;
void Prebuilder::build(Iterator* i){
    for(i->ci(0); i->ci() < i->length()-1; i->operator++()){
        if(i->isK(Keywords::l::para_point)){
            i->operator++();
            if(i->ct() == PARTICLE_CONTEXT){
                i->cc()->setString("." + i->cc()->getString());
                i->erase(i->ci() -1,i->ci());
            }
            i->operator--();
        }
    }
    i->ci(0);
}