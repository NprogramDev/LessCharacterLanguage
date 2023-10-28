#include "genericdt.hpp"
using namespace CompileLCL;
GenericDT::GenericDT(Iterator &i){
this->i = i;
}
unsigned int GenericDT::parse(){
    // TODO Resolve the Generics
    while(true){
        // If the current Keyword is a greater_than => End of the GenericDT or if the current Keyword is a semicolon => End of the command (in case it's an Executor) => break
        if(i.ct() == PARTICLE_KEYWORDS && (i.ck() == Keywords::l::para_greater_than || i.ck() == Keywords::l::para_semicolon))break;
        ++i;
    }
    return i.ci();
}