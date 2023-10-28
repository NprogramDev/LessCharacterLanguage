#ifndef BUILDTREE_HPP_
#define BUILDTREE_HPP_
#include "../limits.hpp"
#include "iterator.hpp"
#include "../simplyfy/particleLine.hpp"
#include "genericdt.hpp"
#include "inheritor.hpp"
#include "subexecutor.hpp"
#include "prebuilder.hpp"
#include <vector>
#include <iostream>
#define ICOID_ICO 0
#define ICOID_EXE 1
#define ICOID_ICOT 2
#define ICOID_VAR 3
#define ICOID_VRE 4
#define ICOID_ICOP 6
#define ICOID_METHOD 7
#define ICOID_CLASS 8
#define ICOID_ROOT 9
namespace CompileLCL{
class ICO{
    public:
    virtual int getID();
    public:
        Iterator i;
        GenericDT* gdt;
    public:
    ICO(Iterator &i);
    virtual unsigned int parse(){return i.ci();}
    void setGenericDT(GenericDT* gdt);
};
class ICOT : virtual public ICO{
    public:
    int getID();
    //private:
    std::vector<Keywords::l> attr;
    std::vector<contextParticle*> annotation;
    std::string name;
    unsigned long long array = -1;
    Keywords::l opOverload = Keywords::l::null;
    public:
    ICOT(Iterator &i);
    bool isAttribute();
    void resolveAttributes();
    void resolveAnnotations();
    void resolveName();
    void resolveArray();
    virtual unsigned int parse();
};
typedef std::vector<ICO*> ICOV;

class VariableEnvironment : virtual public ICO{
    public:
    int getID();
    //private:
    ICOV out;
    public:
    VariableEnvironment(Iterator &i);
    void a(ICO* add);
    void a(ICO* add,GenericDT* gdt);
    bool noVarEnv = false;
    void setNoVariableEnv();
    void parseChildren();
};
class ICOP : public VariableEnvironment,public ICOT{
    public:
    int getID();
    
    public:
    ICOP(Iterator &i);
    
};
class ROOT : public ICOP{
    public:
    int getID();
    public:
    ROOT(Iterator &i);
};
class Executor :public ICO{
    public:
    int getID(); // Returns the ID to parse it from ICO to Executor
    bool startBefore;
    MultiExecutor* mex;
    InnerVRE rawInnerVre;
    VariableEnvironment** resolvedInnerVre;
    size_t resolvedInnerVreLen;
    void parseInnerVRE();
    public:
    Executor(Iterator &i,bool startAt_W_or_J);
    bool commaAsTerminator = false;
    void useCommaAsTerminator(bool use);
    MultiExecutor* getMultiExecutor();
    ITERATOR_DT parse();
};
class Variable : public ICOT{
    public:
    int getID();
    Executor* defaultValEx = nullptr;
    public:
    static bool isEnd(Iterator &i);
    Variable(Iterator &i);
    unsigned int parse();
    bool commaAsTerminator = false;
    void useCommaAsTerminator();
    
};

class Class : public ICOP{
    public:
    int getID();
    std::vector<inheritor*> inheritance;
    public:
    Class(Iterator &i);
    unsigned int parse();
    void resolveInheritance();
};
class Method : public ICOP{
    public:
    int getID();
    public:
    static bool isHere(Iterator &i);
    VariableEnvironment* callbackM;
    std::vector<Variable*> transfers;
    Method(Iterator &i);
    unsigned int parse();
    void resolveTransfers();
    void resolveCallback();

};
ROOT* BuildTree(particleLine pl,bool debug);
void combineVar(Iterator* pl);
}



#endif