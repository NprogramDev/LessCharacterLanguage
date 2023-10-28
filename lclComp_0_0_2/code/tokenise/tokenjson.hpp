#ifndef TOKEN_JSON_HPP_
#define TOKEN_JSON_HPP_



#include <string>
#include "buildtree.hpp"
#include "subexecutor.hpp"
#include "../simplyfy/simplyJson.hpp"
#include "../libs/json.h"

#define TOKEN_JSON_DATA "data"
#define TOKEN_JSON_META "meta"
#define TOKEN_JSON_VERSION_TAG "version"
#define TOKEN_JSON_VERSION 99991
#define TOKEN_JSON_TYPE "type"
#define TOKEN_JSON_TYPE_CLASS "class"
#define TOKEN_JSON_TYPE_EXECUTOR "executor"
#define TOKEN_JSON_TYPE_METHOD "method"
#define TOKEN_JSON_TYPE_VARIABLE "variable"
#define TOKEN_JSON_TYPE_VRE "vre"


namespace CompileLCL{
    std::string tokenJSON(ROOT* root);
    std::vector<json::jobject> tokenizeVRE(VariableEnvironment& vre);
    json::jobject tokenICO(CompileLCL::ICO *ic);

    json::jobject jsonGDT(GenericDT *pDt);
    json::jobject jsonMultiExecutor(MultiExecutor * mex);

    json::jobject jsonExecutor(SubExecutor *&pExecutor);


}


#endif