#include "buildtree.hpp"
using namespace CompileLCL;
/*
 * Constructors
 */
ICO::ICO(Iterator &i) { this->i = i; }
ICOT::ICOT(Iterator &i) : ICO(i) {}
VariableEnvironment::VariableEnvironment(Iterator &i) : ICO(i) {}
ICOP::ICOP(Iterator &i) : ICOT(i), VariableEnvironment(i), ICO(i) {}
ROOT::ROOT(Iterator &i) : ICOP(i), ICO(i) {}
Executor::Executor(Iterator &i, bool startAt_W_or_J) : ICO(i) { this->startBefore = startAt_W_or_J; }
Class::Class(Iterator &i) : ICOP(i), ICO(i) {}
Variable::Variable(Iterator &i) : ICOT(i), ICO(i) {}
Method::Method(Iterator &i) : ICOP(i), ICO(i) {}
/*
 *IDs
 */
int ICO::getID()
{
    return ICOID_ICO;
}
int Executor::getID()
{
    return ICOID_EXE;
}
int ICOT::getID()
{
    return ICOID_ICOT;
}
int ICOP::getID()
{
    return ICOID_ICOP;
}
int VariableEnvironment::getID()
{
    return ICOID_VRE;
}
int Variable::getID()
{
    return ICOID_VAR;
}
int Method::getID()
{
    return ICOID_METHOD;
}
int Class::getID()
{
    return ICOID_CLASS;
}
int ROOT::getID()
{
    return ICOID_ROOT;
}
/*
 * Comma Terminators & Getter type
 */
void Executor::useCommaAsTerminator(bool use)
{
    commaAsTerminator = use;
}
void Variable::useCommaAsTerminator()
{
    commaAsTerminator = true;
}
/*
 * ICO GenericDT
 */
void ICO::setGenericDT(GenericDT *gdt)
{
    this->gdt = gdt;
}
/*
 * Attribute Resolve
 */
bool ICOT::isAttribute()
{
    using namespace Keywords;
    if (i.ct() != PARTICLE_KEYWORDS)
        return false;
    std::vector<Keywords::l> kw{para_public, para_private, para_protected, para_final, para_static, para_abstract, para_transient, para_volatile, para_strictfp, para_goto_allowance};
    for (int x = 0; x < kw.size(); x++)
    {
        if (i.ck() == kw.at(x))
        {
            return true;
        }
    }
    return false;
}
void ICOT::resolveAttributes()
{
    --i;
    while (isAttribute())
    {
        attr.push_back(i.ck());
        --i;
    }
}
/*
 * Annotation Resolve
 */
void ICOT::resolveAnnotations()
{
    --i;
    while (true)
    {
        if (i.ct() != PARTICLE_CONTEXT)
            return;
        --i;
        if (i.ct() != PARTICLE_KEYWORDS)
            return;
        if (i.ck() != Keywords::l::para_annotation)
            return;
        ++i;
        annotation.push_back(i.cc());
    }
}
/*
 * Name Resolve
 */
void ICOT::resolveName()
{
    if (i.ct() != PARTICLE_CONTEXT)
        throw "Name is not Context";
    this->name = i.cc()->getString();
    if (this->name == "op")
    {
        ++i;
        if (i.isOperator())
        {
            opOverload = i.ck();
        }
    }
}
/*
 * Array Resolve
 */
void ICOT::resolveArray()
{
    if (!(i.isK() && i.ck() == Keywords::l::para_bracket_cornered_open))
        return;
    ++i;
    if ((i.isK() && i.ck() == Keywords::l::para_bracket_cornered_close))
    {
        array = 0;
        ++i;
        return;
    }
    if (i.ct() == PARTICLE_NUMB)
    {
        if (i.cn()->getSign() == Keywords::l::positiv_int)
        {
            array = i.cn()->getNumber();
            // BETTER
            ++i;
            ++i;
        }
        else
        {
            throw "Array Index can not be negative nor a float";
        }
    }
}
/*
 * All Objects parsed with Attributes, Annotations, Datatype, Array, Name
 * @return Returns the position of the name
 */
unsigned int ICOT::parse()
{
    unsigned int dtp = i.ci();
    resolveAttributes();
    resolveAnnotations();
    i.ci(dtp);
    ++i;
    resolveArray();
    resolveName();
    return i.ci();
}
/*
 * Class Resolve
 */
void Class::resolveInheritance()
{
    ++i;
    if ((i.isK() && i.ck() == Keywords::l::para_bracket_curly_open))
    {
        --i;
        return;
    }
    if (!(i.isK() && i.ck() == Keywords::l::para_bracket_round_open))
    {
        throw "No () or {}";
    }
    do
    {
        ++i;
        inheritor *s = new inheritor(i);
        i.ci(s->parse());
        inheritance.push_back(s);
    } while (!(i.isK() && i.ck() == Keywords::l::para_bracket_round_close) && (i.isK() && i.ck() == Keywords::l::para_comma));
    if (i.isDebug())
        i.printc("inher");
}
unsigned int Class::parse()
{
    ICOT::parse();
    resolveInheritance();
    ++i;
    this->parseChildren();
    return i.ci();
}
/*
 * Method Resolve
 */
void Method::resolveTransfers()
{
    while (!(i.isK() && i.ck() == Keywords::l::para_bracket_round_close))
    {
        if (i.isSimpleDT() || i.ct() == PARTICLE_CONTEXT)
        {
            Variable *v = new Variable(i);
            v->useCommaAsTerminator();
            i.ci(v->parse());
            transfers.push_back(v);
        }
        ++i;
    }
}
void Method::resolveCallback()
{
    if (!(i.isK() && i.ck() == Keywords::l::para_bracket_cornered_open))
    {
        --i;
        return;
    }
    while (!i.isTriggerType())
    {
        if ((i.isK() && i.ck() == Keywords::l::para_bracket_cornered_close))
        {
            throw "Missing Callback Datatype!";
        }
        ++i;
    }
    unsigned int dtp = i.ci();
    GenericDT *gdt = nullptr;
    if (i.isK() && i.ck() == Keywords::l::para_smaller_than)
    {
        if (i.isDebug())
            std::cout << "Found GenericDT" << std::endl;
        gdt = new GenericDT(i);
        i.ci(gdt->parse());
        // At next keyword after DT
        ++i;
    }
    i.ci(dtp);
    callbackM = nullptr;
    if (i.isClassDT())
    {
        callbackM = new Class(i);
        callbackM->setNoVariableEnv();
    }
    else
    {
        callbackM = new Method(i);
        callbackM->setNoVariableEnv();
    }
    i.ci(callbackM->parse());
    callbackM->setGenericDT(gdt);
    while (!(i.isK() && i.ck() == Keywords::l::para_bracket_cornered_close))
    {
        ++i;
    }
}
unsigned int Method::parse()
{
    ICOT::parse();
    ++i;
    if (!(i.isK() && i.ck() == Keywords::l::para_bracket_round_open))
    {
        throw "No round Bracket in Method declatartion! ";
    }
    resolveTransfers();
    ++i;
    resolveCallback();
    ++i;
    this->parseChildren();
    return i.ci();
}
/*
 * Variable Resolve
 */
unsigned int Variable::parse()
{
    ICOT::parse(); // At name
    ++i;           // Goto next Particle => If this is the end => return
    // i.printc("I'm here");
    if ((i.isK() && (commaAsTerminator ? (i.ck() == Keywords::l::para_comma || i.ck() == Keywords::l::para_bracket_round_close) : i.ck() == Keywords::l::para_semicolon)))
        return i.ci() - 1;
    --i; // Go back to name
    // i.printc("There");
    defaultValEx = new Executor(i, false); // Create ICO Executor
    defaultValEx->useCommaAsTerminator(commaAsTerminator);
    i.ci(defaultValEx->parse());
    return i.ci();
}
/*
 * Add to output of Variable Environment
 */
void VariableEnvironment::a(ICO *add)
{
    i.ci(add->parse());
    out.push_back(add);
}
void VariableEnvironment::a(ICO *add, GenericDT *gdt)
{
    i.ci(add->parse());
    add->setGenericDT(gdt);
    out.push_back(add);
}
void VariableEnvironment::setNoVariableEnv()
{
    noVarEnv = true;
}

/*
 * Detection of type
 */
bool Variable::isEnd(Iterator &i)
{
    if (i.ct() != PARTICLE_KEYWORDS)
        return false;
    return i.ck() == Keywords::l::para_semicolon || i.ck() == Keywords::l::para_set;
}
bool Method::isHere(Iterator &i)
{
    unsigned int dtp = i.ci();
    while (!Variable::isEnd(i))
    {
        ++i;
        if (i.isK() && i.ck() == Keywords::l::para_bracket_curly_open)
        {
            i.ci(dtp);
            return true;
        }
    }
    i.ci(dtp);
    return false;
}
ITERATOR_DT Executor::parse()
{
    if (startBefore)
    {
        while (!(i.isK() && (i.ck() == Keywords::l::para_bracket_curly_open || i.ck() == Keywords::l::para_semicolon)))
        {
            --i;
        }
        ++i;
    }
    mex = new MultiExecutor(&i,&rawInnerVre);
    mex->endOfEx = this->commaAsTerminator ? Keywords::l::para_comma : Keywords::l::para_semicolon;
    this->mex->parse();
    parseInnerVRE();
    return this->mex->endStart-1;
}
void Executor::parseInnerVRE(){
    resolvedInnerVreLen = rawInnerVre.extVec.size();
    resolvedInnerVre = new VariableEnvironment*[resolvedInnerVreLen];
    std::cout << "Resolve Anonymous Functions" << std::endl;
    for(size_t y =1;y < resolvedInnerVreLen; y++){
        std::cout << "Resolve &" << y << std::endl;
        particleLine pl = rawInnerVre.extVec.at(y);
        Iterator jx(pl,i.isDebug());
        while (jx.ci() != jx.length()-1){
            jx.printc("--?");
            ++jx;
        }
        jx.ci(0);
     //   jx.printc("??--?X?");
        resolvedInnerVre[y] = new VariableEnvironment(jx);
        resolvedInnerVre[y]->parseChildren();
    }

}
MultiExecutor *Executor::getMultiExecutor()
{
    return this->mex;
}
/*
 * Resolve every child in a variable environment
 */
int cqc = 0;
void VariableEnvironment::parseChildren()
{
    cqc++;
    int ccq = cqc;
    if (noVarEnv)
        return;
    if (i.ck() == Keywords::l::para_semicolon)
    {
        return;
    }
    if (i.ck() != Keywords::l::para_bracket_curly_open)
        throw "VariableEnvironment doesn't start with brackets!";
    if (i.isDebug())
        i.printc("CURR PARTICLE");
    while (i.length() > i.ci())
    {
        ++i;
        if (i.isDebug())
            std::cout << "[" << std::to_string(ccq) << "]";
        // Stop on }
        if (i.isK() && i.ck() == Keywords::l::para_bracket_curly_close)
        {
            break;
        }
        // Open new VariableEnvironment on {
        if (i.isK(Keywords::l::para_bracket_curly_open))
        {
            a(new VariableEnvironment(i));
            if (i.isDebug())
                std::cout << "Found VRE" << std::endl;
            i.printc("----> VRE");
            continue;
        }
        if (i.isTriggerType())
        {
            unsigned int dtp = i.ci();
            // Must be Executor
            if (i.ct() == PARTICLE_NUMB || i.ct() == PARTICLE_STRING || i.isOtherTrigger())
            {
                i.ci(dtp);
                a(new Executor(i, true));
                if (i.isDebug())
                    std::cout << "Found Executor" << std::endl;
                continue;
            }
            // if(i.isK() && i.ck() == Keywords::l::para_bracket_curly_open){continue;}
            // At next keyword after DT
            if (i.isDebug())
                i.printc("CTC");
            ++i;
            if (i.isDebug())
                i.printc("CTX");
            // Generic Datatype
            GenericDT *gdt = nullptr;
            if (i.isK() && i.ck() == Keywords::l::para_smaller_than)
            {
                if (i.isDebug())
                    std::cout << "Found GenericDT" << std::endl;
                gdt = new GenericDT(i);
                i.ci(gdt->parse());
                // At next keyword after DT
                ++i;
            }
            // Then Executor
            if (i.isDebug())
                i.printc("should be context" + std::to_string(ccq));
            if (i.ct() != PARTICLE_CONTEXT)
            {
                i.ci(dtp);
                a(new Executor(i, true));
                if (i.isDebug())
                    std::cout << "Found Executor" << std::endl;
                continue;
            }
            i.ci(dtp);
            // i.printc("Go back to DT"+ std::to_string(ccq));
            //  if class datatype => create class
            if (i.isClassDT())
            {
                a(new Class(i), gdt);
                if (i.isDebug())
                    std::cout << "Found Class: " << i.ci() << std::endl;
                continue;
            }
            if (Method::isHere(i))
            {
                a(new Method(i), gdt);
                if (i.isDebug())
                    std::cout << "Found Method" << std::endl;
                continue;
            }
            a(new Variable(i), gdt);
            if (i.isDebug())
                std::cout << "Found Variable" << ccq << std::endl;
            continue;
        }
    }
    if (i.isDebug())
        std::cout << "End of I " << i.ci() << std::endl;
}
ROOT *CompileLCL::BuildTree(particleLine pl, bool debug)
{
    Iterator it(pl, debug);
  //  Prebuilder::build(&it);
    combineVar(&it);
    // std::cout << "-- After Iterator --" << std::endl;
    ROOT *r = new ROOT(it);
    // std::cout << "-- Created ROOT --" << std::endl;
    r->parseChildren();
    if (it.isDebug())
        std::cout << "-- After parseChild --" << std::endl;
    return r;
}
void CompileLCL::combineVar(Iterator* i){
 while (i->ci() < i->length()-1)
 {
    i->operator++();
   // i->printc("GD");
    ITERATOR_DT start = i->ci();
    if(i->ct() != PARTICLE_CONTEXT)continue;
    if(i->ci() + 1 >= i->length())break;
    i->operator++();
    if(i->isK(Keywords::l::para_point)){
        if(i->ci() + 1 >= i->length())break;
        i->operator++();
        if(i->ct() != PARTICLE_CONTEXT)continue;
        std::string s = i->cc()->getString();
        i->ci(start);
        //std::cout << "Q: " << i->cc()->getString() << "." << s << std::endl;
        i->cc(i->cc()->getString() + "." + s);
        //i->printc("GQ");
        i->erase(start+1, start+3);
        i->ci(start);
        //i->printc("GP");
        i->operator--();
    }

 }
 for(i->ci(0); i->ci() < i->length()-1;i->operator++()){
    i->printc("POSTPRE");
 }
 i->ci(0);
}