#include "simplyJson.hpp"
using namespace CompileLCL;
#define TYPE "type"
#define CONTEXT_TYPE "CONTEXT"
#define CONTEXT_TEXT "text"
#define KEYWORD_TYPE "KEYWORD"
#define KEYWORD_ID "id"
#define KEYWORD_NAME "name"
#define NUMBER_TYPE "NUMBER"
#define NUMBER_SIGN "sign"
#define NUMBER_VALUE "value"
#define STRING_TYPE "STRING"
#define STRING_TEXT "text"
#define VALUE "value"
#define INNER_EXECUTOR_TYPE "INNER_EXECUTOR"
json::jobject CompileLCL::particleJson(particle* prt){
    json::jobject curr;

    switch (prt->getType())
    {
        case PARTICLE_CONTEXT:
            curr[TYPE] = CONTEXT_TYPE;
            curr[CONTEXT_TEXT] = ((contextParticle*) prt )->getString();
            break;
        case PARTICLE_KEYWORDS:
            curr[TYPE] = KEYWORD_TYPE;
            curr[KEYWORD_ID] = (unsigned long) ((keywordsParticle*) prt)->getKW();
            curr[KEYWORD_NAME] = Keywords::m[((keywordsParticle*) prt)->getKW()];
            break;
        case PARTICLE_NUMB:
            curr[TYPE] = NUMBER_TYPE;
            curr[NUMBER_SIGN] = ((numberParticle*) prt)->getSign();
            curr[NUMBER_VALUE] = (unsigned long long) ((numberParticle*) prt)->getNumber();
            break;
        case PARTICLE_STRING:
            curr[TYPE] = STRING_TYPE;
            curr[STRING_TEXT] = ((stringParticle*) prt)->getString();
            break;
        case PARTICLE_INNER_EXECUTOR:
            curr[TYPE] = INNER_EXECUTOR_TYPE;
            curr[VALUE] = ((InExecutorVariable*) prt)->getVariableID();
            break;
        default:
            std::cout << ".. Default! .. " << prt->getType() << std::endl;
    }
    return curr;
}

std::string CompileLCL::simplyJSON(particleLine particles){
    json::jobject rt;
    
    std::vector<json::jobject> particleVec;
    for(auto it = particles.begin(); it != particles.end(); ++it){
        particleVec.push_back(particleJson(*it));
    }
    rt["particles"] = particleVec;
    rt["count"] = (unsigned int) particleVec.size();
    return rt;
}