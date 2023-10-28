#ifndef ITERATOR_HPP_
#define ITERATOR_HPP_
#include <vector>
#include <iostream>
#include "../limits.hpp"
#include "../simplyfy/particleLine.hpp"
#include "../simplyfy/classify.hpp"
namespace CompileLCL
{
    class Iterator
    {
    private:
        bool isNull = true;
        bool debug;
        unsigned int iterator = 0;
        particleLine particles;

    public:
        Iterator(particleLine &particles, bool debug);
        Iterator();
        bool executor_jumpBrackets(bool includeCase=false);
        unsigned int operator++();
        unsigned int operator--();
        particle *cp();
        int ct();
        Keywords::l ck();
        contextParticle *cc();
        void cc(std::string txt);
        numberParticle *cn();
        stringParticle *cs();
        void printc(std::string msg);
        unsigned int ci();
        void ci(unsigned int iterator);
        void insertParticle(particle *particle);
        particleLine* getParticleLine();
        size_t length();
        bool isK();
        bool isK(Keywords::l keyw);
        bool isTriggerType();
        bool isSimpleDT();
        bool isClassDT();
        bool isOtherTrigger();
        bool isOperator();
        bool isOperatorSet();
        bool isDebug();
        void erase(ITERATOR_DT start, ITERATOR_DT end);
        bool isFlowControlKW();
    };
}
#endif