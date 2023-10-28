#ifndef SIMPLY_JSON_HPP_
#define SIMPLY_JSON_HPP_
#include "particleLine.hpp"
#include <string>
#include <vector>
#include "../libs/json.h"
#include "classify.hpp"
#include "../tokenise/subexecutor.hpp"
namespace CompileLCL{
    std::string simplyJSON(particleLine particles);
    json::jobject particleJson(particle *prt);
}
#endif