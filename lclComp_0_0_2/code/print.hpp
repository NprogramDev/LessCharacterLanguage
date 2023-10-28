#ifndef PRINT_HPP_
#define PRINT_HPP_
#include "simplyfy/keywords.hpp"
#include "simplyfy/simply.hpp"
#include "simplyfy/classify.hpp"
#include "helper.hpp"
#include "tokenise/buildtree.hpp"
#include "tokenise/inheritor.hpp"
#include "tokenise/subexecutor.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include "helper.hpp"

namespace CompileLCL
{

    void printParticleLine(particleLine p);
    void printBinaryStr(std::string prt);
    int printBinaryKeywords(std::string binarys);
    void printR(VariableEnvironment *r, int depth);
    void printROOTExecutor(Executor *exe, int depth);
}
#endif