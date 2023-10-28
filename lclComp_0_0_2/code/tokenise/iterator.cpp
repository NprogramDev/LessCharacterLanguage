#include "iterator.hpp"
#include <limits>
using namespace CompileLCL;
Iterator::Iterator()
{
}
int bcc = 0;
bool Iterator::executor_jumpBrackets(bool includeCase){
    if(this->ct() != PARTICLE_KEYWORDS) return false;
    Keywords::l closeKW = Keywords::l::null;
    switch (this->ck())
    {
        case Keywords::l::para_bracket_round_open:
        closeKW = Keywords::l::para_bracket_round_close;
        break;
        case Keywords::l::para_bracket_cornered_open:
        closeKW = Keywords::l::para_bracket_cornered_close;
        break;
        case Keywords::l::para_bracket_curly_open:
        closeKW = Keywords::l::para_bracket_curly_close;
        break;
        case Keywords::l::para_case:
            if(!includeCase) return false;
            closeKW = Keywords::l::para_semicolon;
                    break;
        default:
        return false;
    }
    bcc++;
    std::cout << "BCC: " << bcc << std::endl;
    while(!(this->ct() == PARTICLE_KEYWORDS && this->ck() == closeKW)){
        this->operator++();
        this->executor_jumpBrackets();
        //std::cout << "JUMP!" << std::endl;
    }
    this->operator++();
    return true;
}
unsigned int Iterator::operator++()
{
    if (isNull)
        return std::numeric_limits<unsigned int>::max();
    if (iterator + 1 >= this->particles.size())
        throw "Tree Iterator out of range!" + std::to_string(iterator + 1);
    iterator++;
    return iterator;
}
unsigned int Iterator::operator--()
{
    //   std::cout << "i-0;" << std::endl;
    if (isNull)
        return std::numeric_limits<unsigned int>::max();
    // std::cout << "i-1;" << std::endl;
    if (iterator == 0)
        return iterator;
    iterator--;
    // std::cout << "i--;" << std::endl;
    return iterator;
}
Iterator::Iterator(particleLine &particleLine,bool debug)
{
    this->debug = debug;
    this->particles = particleLine;
    this->isNull = false;
    // std::cout << "with Particle" << std::endl;
}
bool Iterator::isDebug(){
    return this->debug;
}
particle *Iterator::cp()
{
    if (isNull)
        return nullptr;
    return this->particles.at(iterator);
}
int Iterator::ct()
{
    if (isNull)
        return -1;
    return cp()->getType();
}
Keywords::l Iterator::ck()
{
    if (ct() != PARTICLE_KEYWORDS)
    {
        throw "Keyword not matching!";
        return Keywords::l::null;
    }
    return ((keywordsParticle *)cp())->getKW();
}
void Iterator::cc(std::string txt){
    if (ct() != PARTICLE_CONTEXT)
    {
        throw "Keyword not matching!";
    }
    ((contextParticle *)cp())->setString(txt);
}
contextParticle *Iterator::cc()
{
    if (ct() != PARTICLE_CONTEXT)
    {
        throw "Keyword not matching!";
        return nullptr;
    }
    return (contextParticle *)cp();
}
numberParticle *Iterator::cn()
{
    if (ct() != PARTICLE_NUMB)
    {
        throw "Keyword not matching!";
        return nullptr;
    }
    return (numberParticle *)cp();
}
stringParticle *Iterator::cs()
{
    if (ct() != PARTICLE_STRING)
    {
        throw "Keyword not matching!";
        return nullptr;
    }
    return (stringParticle *)cp();
}
unsigned int Iterator::ci()
{
    return iterator;
}
void Iterator::ci(unsigned int iterator)
{
    if (iterator >= this->particles.size())
        throw "Tree Iterator out of range!" + std::to_string(iterator);
    this->iterator = iterator;
}
bool Iterator::isSimpleDT()
{
    if (ct() != PARTICLE_KEYWORDS)
    {
        return false;
    }

    using namespace Keywords;
    Keywords::l w = ck();
    std::vector<Keywords::l> kw{para_int, para_boolean, para_char, para_string, para_byte, para_float, para_long, para_double, para_short, para_object, para_void, para_ab_obj_cl};
    for (int i = 0; i < kw.size(); i++)
    {
        if (w == kw.at(i))
        {
            return true;
        }
    }
    return false;
}
void Iterator::printc(std::string msg)
{
    switch (ct())
    {
    case PARTICLE_CONTEXT:
        std::cout << msg << "[CURRENT-CONTEXT] " << cc()->getString() << std::endl;
        break;
    case PARTICLE_KEYWORDS:
        std::cout << msg << "[CURRENT-KEYWORD] " << Keywords::m[ck()] << std::endl;
        break;
    case PARTICLE_NUMB:
        std::cout << msg << "[CURRENT-NUMBER] " << cn()->getNumber() << std::endl;
        break;
    case PARTICLE_STRING:
        std::cout << msg << "[CURRENT-STRING] " << cs()->getString() << std::endl;
        break;
    case PARTICLE_INNER_EXECUTOR:
        std::cout << msg << "[INNER-EXECUTOR] " << "Can not show it!" << std::endl;
        break;
    }
}
bool Iterator::isTriggerType()
{
    using namespace Keywords;
    if(ct() == PARTICLE_STRING || ct() == PARTICLE_NUMB) return true;
    if (ct() == PARTICLE_CONTEXT)
    {
        return true;
    }
    return isClassDT() || isSimpleDT() || isOtherTrigger();
}
bool Iterator::isOtherTrigger()
{
    using namespace Keywords;
    if (ct() != PARTICLE_KEYWORDS)
    {
        return false;
    }
    Keywords::l w = ck();
    std::vector<Keywords::l> kw{para_if,
                                para_else,
                                para_for,
                                para_while,
                                para_do,
                                para_switch,
                                para_if_end,
                                para_return,
                                para_break,
                                para_continue,
                                para_case,
                                para_default,
                                para_goto
                              /*  para_bracket_round_open*/
                                };
    for (int i = 0; i < kw.size(); i++)
    {
        if (w == kw.at(i))
        {
            return true;
        }
    }
    return false;
}
bool Iterator::isClassDT()
{
    using namespace Keywords;
    if (ct() != PARTICLE_KEYWORDS)
    {
        return false;
    }
    Keywords::l w = ck();
    std::vector<Keywords::l> kw{para_class, para_interface, para_nativ, para_enum, para_record};
    for (int i = 0; i < kw.size(); i++)
    {
        if (w == kw.at(i))
        {
            return true;
        }
    }
    return false;
}
bool Iterator::isOperator()
{
    using namespace Keywords;
    std::vector<Keywords::l> kw{para_plus,
			para_plus_set,
			para_plus_plus,
			para_minus,
			para_minus_set,
			para_minus_minus,
			para_multiply,
			para_multiply_set,
			para_divide,
			para_divide_set,
			para_modulo,
			para_modulo_set,
			//
			para_set,
			para_equal,
			para_not_equal,
			para_greater_than,
			para_smaller_than,
			para_greater_equal,
			para_smaller_equal,
			para_not,
			para_and_and,
			para_or_or,
			para_xor,
			para_unary,
			para_right_shift,
			para_right_right_shift,
			para_left_shift,
			para_and_set,
			para_or_set,
			para_or,
			para_and,
			para_xor_set,
			para_right_shift_set,
			para_right_right_shift_set,
			para_left_shift_set,
			para_object_or,
			};
    for (int i = 0; i < kw.size(); i++)
    {
        if (this->ck() == kw.at(i))
        {
            return true;
        }
    }
    return false;
}
bool Iterator::isOperatorSet(){
    using namespace Keywords;

    std::vector<Keywords::l> kw{
			para_plus_set,
			para_plus_plus,
			para_minus_set,
			para_minus_minus,

			para_multiply_set,

			para_divide_set,

			para_modulo_set,
			//
			para_set,
			para_and_set,
			para_or_set,
			para_xor_set,
			para_right_shift_set,
			para_right_right_shift_set,
			para_left_shift_set,
    };
    for (int i = 0; i < kw.size(); i++)
    {
        if (this->ck() == kw.at(i))
        {
            return true;
        }
    }
    return false;
}
bool Iterator::isK(){
    return ct() == PARTICLE_KEYWORDS;
}
bool Iterator::isK(Keywords::l keyw){
    return isK() && ck() == keyw;
}
void Iterator::insertParticle(particle* particle){
    this->particles.at(this->ci()) = particle;
}
void Iterator::erase(ITERATOR_DT start,ITERATOR_DT end){
    particles.erase(std::next(particles.begin(), start), std::next(particles.begin(),end));
    ci(start);
}
bool Iterator::isFlowControlKW(){
    return this->isK(Keywords::l::para_for) || this->isK(Keywords::l::para_while) || this->isK(Keywords::l::para_do) ||  this->isK(Keywords::l::para_switch) || this->isK(Keywords::l::para_case);
}
size_t Iterator::length(){
    return this->particles.size();
}
particleLine* Iterator::getParticleLine(){
    return &this->particles;
}