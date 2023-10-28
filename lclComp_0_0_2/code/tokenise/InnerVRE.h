//
// Created by panin on 27.10.2023.
//

#ifndef INNERVRE_H
#define INNERVRE_H

#include <vector>
#include "../simplyfy/particleLine.hpp"
#include "./iterator.hpp"

namespace CompileLCL{
    typedef size_t InnerVreID;
class InnerVRE {
    public:

        InnerVRE();
        size_t add(Iterator* i, size_t startIndex,bool goBack);
    std::vector<std::vector<CompileLCL::particle*>> extVec;

};

}
#endif //YOURPROJECTNAME_INNERVRE_H
