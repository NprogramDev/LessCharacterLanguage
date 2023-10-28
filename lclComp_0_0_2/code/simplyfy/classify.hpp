#ifndef CLASSIFY_HPP_
#define CLASSIFY_HPP_
#include <vector>
#include <string>
#include "keywords.hpp"
#include "../helper.hpp"
#include "particleLine.hpp"

namespace CompileLCL{
    
    typedef unsigned long long bStorageU;
    
    class numberParticle : public particle{
        bStorageU number;
        unsigned int sign;
        public:
        numberParticle(bStorageU number,unsigned int sign);
        bStorageU getNumber();
        unsigned int getSign();
        
    };
    class contextParticle : public particle{
        std::string txt;
        public:
            contextParticle(std::string txt);
            std::string getString();
            void setString(std::string txt);
    };
    class stringParticle : public particle{
        std::string txt;
        public:
            stringParticle(std::string txt);
            std::string getString();

    };
    class keywordsParticle : public particle{
        public:
            keywordsParticle(unsigned int kw);
            Keywords::l getKW();
        private:
            unsigned int kw;
        public:
            bool operator == (unsigned int cp);
    };
    particleLine classify(std::string binary);
};
#endif