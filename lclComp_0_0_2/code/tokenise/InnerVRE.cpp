//
// Created by panin on 27.10.2023.
//

#include "InnerVRE.h"
using namespace CompileLCL;
InnerVRE::InnerVRE(){
    extVec.push_back(std::vector<particle*>());
}
size_t InnerVRE::add(Iterator *i, size_t startIndex,bool goBack) {
    size_t leng = 0;
    ITERATOR_DT start = i->ci();
    i->ci(startIndex);
    while(i->ci() != i->length() && !i->isK(Keywords::l::para_bracket_curly_close)){
        i->operator++();
        i->printc("SAS");
        i->executor_jumpBrackets();
        i->printc("SASC");
        if(i->isK(Keywords::l::para_bracket_curly_close))break;
    }
    leng = i->ci()-startIndex;
    i->ci(startIndex);
    //particle** rt = new particle*[leng];
    std::vector<particle*> rt = std::vector<particle*>(leng+2);
    rt[0] = new keywordsParticle(Keywords::l::para_bracket_curly_open);
    rt[leng+1] = new keywordsParticle(Keywords::l::para_bracket_curly_close);
    for(size_t q = 1; q < leng+1; q++){
        i->operator++();
        rt[q] = i->cp();
        std::cout << q << " " << leng << std::endl;
        i->printc("SASX");
    }
    //i->operator++();
    if(goBack)i->ci(start);
    this->extVec.push_back(rt);
    return extVec.size()-1;
}
