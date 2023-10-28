//
// Created by panin on 20.09.2023.
//

#include "betterJSON.h"
#include <sstream>
using namespace CompileLCL;
std::string getSpaces(int depth){
    std::stringstream strs;
    for(size_t i =0; i < depth; i++){
        strs << " ";
    }
    return  strs.str();
}

std::string CompileLCL::beatiful(std::string json) {
    int depth = 0;
    std::stringstream sts;
    for(size_t i = 0; i < json.length(); i++){
        if(json.at(i) == '}'){
            depth--;
            sts << std::endl;
            sts << getSpaces(depth);
        }
        if(json.at(i) ==']'){
            depth--;
        }
        sts << json.at(i);
        if(json.at(i) == '\n'){
            sts << getSpaces(depth);
        }
        if(json.at(i) == ',' || json.at(i) == '{' || json.at(i) == '['){
            if(json.at(i) == '{' || json.at(i) == '['){
                depth++;
            }
            sts << std::endl;
            sts << getSpaces(depth);
        }
    }
    json = sts.str();
    return sts.str();
}