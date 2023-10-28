#ifndef SUBEXECUTOR_HPP_
#define SUBEXECUTOR_HPP_
#include "../limits.hpp"
#include "iterator.hpp"
#include "../simplyfy/keywords.hpp"
#include "InnerVRE.h"
#include <list>
#define EXECUTOR_SUB_EXECUTOR -1
#define EXECUTOR_MULTIEXECUTOR 1
#define EXECUTOR_METHOD 2
#define EXECUTOR_OPERATOR 3
#define EXECUTOR_FLOWCONTROL 4

#define SUBEXECUTOR_SWITCH_FINAL 5

namespace CompileLCL{
    class SubExecutor{
        protected:
        Iterator* i;
        int sbtype;
        InnerVRE* innerVre;
        public:
            SubExecutor(Iterator* i,InnerVRE* innerVre );
            void printC();
            int getType();
            virtual void parse();
            ITERATOR_DT ci();
            unsigned int cixi();
    };
    class InExecutorVariable : public particle{
        unsigned int variableID;
        public:
        InExecutorVariable(unsigned int variableID);
        unsigned int getVariableID();
    };
    class MultiExecutor : public SubExecutor{
        bool scanForStart;
        public:
        static Keywords::l OPERATOR_ORDER[];
        MultiExecutor(Iterator* i,InnerVRE* innerVre);
        ITERATOR_DT start,endStart,end;
        unsigned int result,innerResult;
        Keywords::l endOfEx;
        std::vector<SubExecutor*> internalExecutors;
        std::vector<particle*> varMapping;
        void setScanForStart();
        void resolveLength();
        void replaceVars();
        void resolveOperators();
        void postProcessor();
        bool resolveCast();
        void resolveExecutors();
        void resolveMultiEx();
        void resolveMethod();
        void print();
        void goUntil(Keywords::l kw);
        void parse();
    };
    class OperatorExecutor : public SubExecutor{
        public:
        OperatorExecutor(Iterator* i,InnerVRE* innerVre );
        unsigned int param1,param2;
        unsigned int result;
        Keywords::l opera;
        void parse();
    };
    enum MethodType{
        IF,
        WHILE,
        FOR,
        DO,
        CUSTOM
    };
    class MethodExecutor : public SubExecutor{
        public:
        MethodExecutor(Iterator* i,InnerVRE* innerVre );
        std::vector<MultiExecutor*> params;
        unsigned int result;
        MethodType type;
        InnerVreID ivId;
        unsigned int namePath;
        void parse();
    };
    enum FlowControlTypes{
        L_UNDEF,
        L_WHILE,
        L_FOR,
        L_DO,
        L_SWITCH,
        L_CASE
    };
    class FlowControlExecutor : public SubExecutor{
    public:
        FlowControlExecutor(Iterator* i,InnerVRE* innerVre );
        void parse();
        MultiExecutor** exc;
        FlowControlTypes flT;
        InnerVreID ivId;
        InnerVreID headerIvID; // Can also just indicate if switch is final or not <=> final switch
        unsigned int executerCount;
        unsigned int result;
    };
}
#endif