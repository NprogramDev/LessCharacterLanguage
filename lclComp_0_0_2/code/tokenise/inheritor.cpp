#include "inheritor.hpp"
using namespace CompileLCL;

inheritor::inheritor(Iterator &i){
    this->i = i;
}
unsigned int inheritor::parse(){
    if(!i.isSimpleDT() && !i.isClassDT()){
        throw "Inheritor is not a Datatype!";
    }
    this->pc = i.cp();
    ++i;
    if((i.ct() == PARTICLE_KEYWORDS && i.ck() == Keywords::l::para_smaller_than)){
        this->gdt = new GenericDT(i);
        i.ci(gdt->parse());
        ++i;
    }
    resolveArray();
    return i.ci();
}
void inheritor::resolveArray(){
    if(!(i.ct() == PARTICLE_KEYWORDS && i.ck()== Keywords::l::para_bracket_cornered_open))return;
    ++i;
    if((i.ct() == PARTICLE_KEYWORDS && i.ck()== Keywords::l::para_bracket_cornered_close)){
        array = 0;
        ++i;
        return;
    }
    if(i.ct() == PARTICLE_NUMB){
        if(i.cn()->getSign() == Keywords::l::positiv_int){
            array = i.cn()->getNumber();
            // BETTER
            ++i;
            ++i;
        }else{
            throw "Array Index can not be negative nor a float";
        }
    }

}
particle* inheritor::getParticle(){
    return this->pc;
}