#include "classify.hpp"

using namespace CompileLCL;
/*
int getUNDEFType()
{
    return PARTICLE_UNDEF;
}
int getNUMType()
{
    return PARTICLE_NUMB;
}
int getCXType()
{
    return PARTICLE_CONTEXT;
}
int getKWType()
{
    return PARTICLE_KEYWORDS;
}
int getStrType()
{
    return PARTICLE_STRING;
}*/
particle::particle(){
    this->type = PARTICLE_UNDEF;
}
numberParticle::numberParticle(bStorageU number,unsigned int sign) : particle(){
    this->number =number;
    this->sign = sign;
    this->type = PARTICLE_NUMB;
}
contextParticle::contextParticle(std::string txt) : particle(){
    this->txt = txt;
    this->type = PARTICLE_CONTEXT;
}
stringParticle::stringParticle(std::string txt) : particle(){
    this->txt = txt;
    this->type = PARTICLE_STRING;
}
keywordsParticle::keywordsParticle(unsigned int kw) : particle(){
    this->kw = kw;
    this->type = PARTICLE_KEYWORDS;
}
int particle::getType(){
    return type;
}
bStorageU numberParticle::getNumber(){
            return this->number;
        }
unsigned int numberParticle::getSign(){
            return this->sign;
        }
std::string contextParticle::getString(){
    return this->txt;
}
void contextParticle::setString(std::string txt){
    this->txt = txt;
}
std::string stringParticle::getString(){
    return this->txt;
}
Keywords::l keywordsParticle::getKW(){
    return (Keywords::l) this->kw;
}

bool keywordsParticle::operator==(unsigned int cp)
{
    return kw = cp;
}





particleLine CompileLCL::classify(std::string binary){
    particleLine rt;
    int iterator = 0;
    while(iterator < binary.length()){
        if(binary.at(iterator) == Keywords::l::context_string){
            iterator++;
            std::string s = "";
            while(binary.at(iterator) != Keywords::l::context_string){
                s += binary.at(iterator);
                iterator++;
            }
            rt.push_back(new contextParticle(s));
        }else
        if(binary.at(iterator) == Keywords::l::actual_string){
            iterator++;
            std::string s = "";
            while(binary.at(iterator) != Keywords::l::actual_string){
                s += binary.at(iterator);
                iterator++;
            }
            rt.push_back(new stringParticle(s));
        }else
        if(binary.at(iterator) == Keywords::l::number){
            iterator++;
            unsigned int sign = binary.at(iterator);
            iterator++;
            
            std::string q = "";
            while (binary.at(iterator) != Keywords::l::number)
            {
                unsigned char x = binary.at(iterator);
                x -= (Keywords::l::number +1);
                q += x;
                iterator++;
            }
            bStorageU number = stringTNumber(128,q);
            rt.push_back(new numberParticle(number,sign));
        }else rt.push_back(new keywordsParticle(binary.at(iterator)));
        iterator++;
    }
    return rt;
}