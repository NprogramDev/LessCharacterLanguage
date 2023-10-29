#include "subexecutor.hpp"
using namespace CompileLCL;

SubExecutor::SubExecutor(Iterator *i,InnerVRE* innerVre )
{
    this->i = i;
    this->sbtype = -1;
    this->innerVre = innerVre;
}
int SubExecutor::getType()
{
    return this->sbtype;
}
ITERATOR_DT SubExecutor::ci()
{
    return this->i->ci();
}
unsigned int SubExecutor::cixi()
{
    return ((InExecutorVariable *)i->cp())->getVariableID();
}
void SubExecutor::parse()
{ /* Overridden*/
}
MultiExecutor::MultiExecutor(Iterator *i,InnerVRE* innerVre ) : SubExecutor(i,innerVre) { this->sbtype = EXECUTOR_MULTIEXECUTOR; }
MethodExecutor::MethodExecutor(Iterator *i,InnerVRE* innerVre ) : SubExecutor(i,innerVre) { this->sbtype = EXECUTOR_METHOD; }
OperatorExecutor::OperatorExecutor(Iterator *i,InnerVRE* innerVre ) : SubExecutor(i,innerVre) { this->sbtype = EXECUTOR_OPERATOR; }
FlowControlExecutor::FlowControlExecutor(CompileLCL::Iterator *i,InnerVRE* innerVre ) : SubExecutor(i,innerVre) {this->sbtype = EXECUTOR_FLOWCONTROL;}
InExecutorVariable::InExecutorVariable(unsigned int variableID)
{
    this->type = PARTICLE_INNER_EXECUTOR;
    this->variableID = variableID;
}
unsigned int InExecutorVariable::getVariableID()
{
    return this->variableID;
}
void OperatorExecutor::parse()
{ // At the operator
    if (!i->isK())
        throw "OperatorExecutor::parse()";
    this->opera = i->ck();
    i->operator--();
    if (i->ct() == PARTICLE_INNER_EXECUTOR)
    {
        this->param1 = cixi();
    }
    else
    {
        this->param1 = 0;
    }
    i->operator++();
    i->operator++();
    if (i->ct() == PARTICLE_INNER_EXECUTOR)
    {
        this->param2 = cixi();
    }
    else
    {
        this->param2 = 0;
    }
}
void SubExecutor::printC()
{
    if (!i->isDebug())
        return;
    switch (i->ct())
    {
    case PARTICLE_CONTEXT:
        std::cout << ((contextParticle *)i->cp())->getString();
        break;
    case PARTICLE_INNER_EXECUTOR:
        std::cout << "$" << ((InExecutorVariable *)i->cp())->getVariableID();
        break;
    case PARTICLE_KEYWORDS:
        std::cout << Keywords::m[(((keywordsParticle *)i->cp())->getKW())];
        break;
    case PARTICLE_NUMB:
        std::cout << ((numberParticle *)i->cp())->getNumber();
        break;
    case PARTICLE_STRING:
        std::cout << ((stringParticle *)i->cp())->getString();
    }
    std::cout << "  §" << i->ci() << std::endl;
}

void MultiExecutor::resolveLength()
{
    // if(i->isK() && i->ck() == Keywords::l::para_bracket_round_open)i->operator++();
    std::cout << "RESOLVE LENGTH -> START" << std::endl;
    printC();
    start = i->ci();
    while (!(i->isK() && i->ck() == endOfEx))
    {
        // printC();
        std::cout << "RESOLVE LENGTH -> X1" << std::endl;
        printC();
        if (i->executor_jumpBrackets()) //=> JUMPED
        {
            i->operator--();
            if(i->isK(Keywords::l::para_bracket_curly_close))break;
                i->operator++();
        }else{ // => NOT JUMPED
            i->operator++();
        }
        if (i->isK() && i->ck() == Keywords::l::para_bracket_round_close){
            std::cout << "Length stopped at )" << std::endl;
            break;
        }
        if(i->isK(Keywords::l::para_bracket_curly_close)){
            std::cout << "Length stopped at }" << std::endl;
            break;
        }

    }
    // std::cout << "--C--" << std::endl;
    end = i->ci();
    endStart = end;
    i->ci(start);
    // std::cout << "--D--" << std::endl;
}
void MultiExecutor::replaceVars()
{
    varMapping.push_back(nullptr);
    while (i->ci() < end)
    {
        switch (i->ct())
        {
        case PARTICLE_CONTEXT:
        case PARTICLE_NUMB:
        case PARTICLE_STRING:
            particle *pc = new InExecutorVariable(varMapping.size());
            varMapping.push_back(i->cp());
            i->insertParticle(pc);
        }
        if (!i->executor_jumpBrackets(true))
        {
            i->operator++();
        }
    }
}
Keywords::l MultiExecutor::OPERATOR_ORDER[] = {
    Keywords::l::para_pointer_addr,
    Keywords::l::para_pointer_value,
    Keywords::l::para_point,
    Keywords::l::para_plus_plus,
    Keywords::l::para_minus_minus,
    Keywords::l::para_unary,
    Keywords::l::para_not,
    Keywords::l::para_power_of,
    Keywords::l::para_multiply,
    Keywords::l::para_divide,
    Keywords::l::para_modulo,
    Keywords::l::para_plus,
    Keywords::l::para_minus,
    Keywords::l::para_if,
    Keywords::l::para_else,
    /*Keywords::l::para_for,*/
    /*Keywords::l::para_while,*/
    Keywords::l::para_do,
    /*Keywords::l::para_switch,*/
    Keywords::l::para_if_end,
    /*Keywords::l::para_case,*/
    Keywords::l::para_default,
    Keywords::l::para_goto,
    Keywords::l::para_equal,
    Keywords::para_not_equal,
    Keywords::l::para_and,
    Keywords::l::para_or,
    Keywords::l::para_xor,
    Keywords::l::para_and_and,
    Keywords::l::para_or_or,
    Keywords::l::para_or_set,
    Keywords::l::para_and_set,
    Keywords::l::para_xor_set,
    Keywords::l::para_multiply_set,
    Keywords::l::para_divide_set,
    Keywords::l::para_modulo_set,
    Keywords::l::para_plus_set,
    Keywords::l::para_minus_set,
    Keywords::l::para_set,
    Keywords::l::para_return,
    Keywords::l::para_break,
    Keywords::l::para_continue,
    Keywords::l::para_package,
    Keywords::l::para_import,
    Keywords::l::para_extern
    };
void MultiExecutor::goUntil(Keywords::l kw)
{
    while (i->ci() < end && !(i->isK() && i->ck() == kw))
    {
        i->operator++();
    }
}
bool MultiExecutor::resolveCast()
{
    ITERATOR_DT startX = i->ci();
    if (!i->executor_jumpBrackets())
    {
        i->operator++();
    }
    if (i->ct() != PARTICLE_INNER_EXECUTOR)
    {
        i->ci(startX);
        return false;
    }
    i->ci(startX);
    OperatorExecutor *opx = new OperatorExecutor(i,innerVre);
    i->operator++();
    ITERATOR_DT dtp = i->ci();
    // Replace Var
    varMapping.push_back(nullptr);
    while (i->ci() < end && !(i->isK() && i->ck() == Keywords::l::para_bracket_round_close))
    {
        switch (i->ct())
        {
        case PARTICLE_CONTEXT:
        case PARTICLE_NUMB:
        case PARTICLE_STRING:
            particle *pc = new InExecutorVariable(varMapping.size());
            varMapping.push_back(i->cp());
            i->insertParticle(pc);
        }
        if (!i->executor_jumpBrackets())
        {
            i->operator++();
        }
    }
    i->ci(dtp);
    // print();
    if (i->ct() != PARTICLE_INNER_EXECUTOR)
        throw "No Inner Executor in Cast!";
    opx->param1 = cixi();
    i->operator++();
    if (!(i->isK() && i->ck() == Keywords::l::para_bracket_round_close))
        throw "Cast has to many Values in Brackets!";
    i->operator++();
    if (!(i->ct() == PARTICLE_INNER_EXECUTOR))
        throw "Should never be executed in Cast Resolve, because definition of Cast is Inner Executor Particle behind brackets";
    opx->param2 = cixi(); // At the end of cast
    opx->opera = Keywords::l::para_bracket_round_open;
    InExecutorVariable *iexV = new InExecutorVariable(varMapping.size());
    varMapping.push_back(iexV);
    opx->result = iexV->getVariableID();
    end += (startX - i->ci());
    // std::cout << "ENDX: " << (startX-i->ci()) << std::endl;
    i->erase(startX, i->ci());
    i->insertParticle(iexV);
    internalExecutors.push_back(opx);
    print();
    return true;
}
void MultiExecutor::print()
{
    if (!i->isDebug())
        return;
    std::cout << std::endl;
    ITERATOR_DT startX = i->ci();
    i->ci(start);
    while (i->ci() < end)
    {
        switch (i->ct())
        {
        case PARTICLE_CONTEXT:
            std::cout << ((contextParticle *)i->cp())->getString();
            break;
        case PARTICLE_INNER_EXECUTOR:
            std::cout << "$" << ((InExecutorVariable *)i->cp())->getVariableID();
            break;
        case PARTICLE_KEYWORDS:
            std::cout << Keywords::m[(((keywordsParticle *)i->cp())->getKW())];
            break;
        case PARTICLE_NUMB:
            std::cout << ((numberParticle *)i->cp())->getNumber();
            break;
        case PARTICLE_STRING:
            std::cout << ((stringParticle *)i->cp())->getString();
        }
        std::cout << "   ";
        i->operator++();
    }
    i->ci(startX);
    std::cout << std::endl;
}
void MultiExecutor::resolveMethod()
{
    // Generate and Map the Variable, that is the result of this executor
    InExecutorVariable *iexV = new InExecutorVariable(varMapping.size());
    varMapping.push_back(iexV);
    // Get the name of the method and replace it by the result
    InExecutorVariable *name = (InExecutorVariable *)i->cp();
    i->insertParticle(iexV);
    // Get the starting-Position of the Method
    ITERATOR_DT startX = i->ci();
    MethodExecutor *meth = new MethodExecutor(i,innerVre);
    meth->result = iexV->getVariableID();
    meth->namePath = name->getVariableID();
    meth->ivId =0;
    if (i->isDebug())
        std::cout << "--3--" << std::endl;
    meth->parse();
    end += ((startX + 1) - i->ci());
    i->erase(startX + 1, i->ci());
    print();
    printC();
    internalExecutors.push_back(meth);
}
void MultiExecutor::resolveMultiEx()
{
    InExecutorVariable *iexV = new InExecutorVariable(varMapping.size());
    varMapping.push_back(iexV);
    MultiExecutor *mx = new MultiExecutor(i,innerVre);
    mx->endOfEx = Keywords::l::para_bracket_round_close;
    mx->result = iexV->getVariableID();
    // std::cout << "--6--" << std::endl;
    i->operator++();
    mx->parse();
    i->ci(mx->start - 1);
    i->insertParticle(iexV);
    i->erase(mx->start, mx->end + 1);
    end += (mx->start - mx->endStart) - 1;
    if (i->isDebug())
        std::cout << "--7--" << std::endl;
    print();
    if (i->isDebug())
        std::cout << "------" << std::endl;
    internalExecutors.push_back(mx);
}
void FlowControlExecutor::parse() {
    switch (i->ck()) {
        case Keywords::l::para_while:{
            flT = L_WHILE;
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_round_open)){throw "Error! While is missing '('";}
            i->operator++();
            exc = new MultiExecutor*[1];
            executerCount =1;
            exc[0] = new MultiExecutor(i,innerVre);
            exc[0]->endOfEx = Keywords::l::para_bracket_round_close;
            exc[0]->parse();
            if(!i->isK(Keywords::l::para_bracket_round_close)){throw "WHy is there no ')' after the while?";}
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_curly_open)){throw "While is missing {";}
            ivId = innerVre->add(i,i->ci(),false);   // Back to VRE
            break;
        }
        case Keywords::l::para_for:{
            flT = L_FOR;

            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_round_open)) {throw "For has not (";}
            ITERATOR_DT start = i->ci();
            i->executor_jumpBrackets();
            i->operator--();
            if(!i->isK(Keywords::l::para_bracket_round_close)) {throw "For has no )";}
            i->insertParticle(new keywordsParticle(Keywords::l::para_bracket_curly_close));
            i->ci(start);
            i->insertParticle(new keywordsParticle(Keywords::l::para_bracket_curly_open));
            headerIvID = innerVre->add(i,i->ci(),false);
            i->operator++();
            i->printc("XCCCCCCCC");
            if(!i->isK(Keywords::l::para_bracket_curly_open)) {throw "For has not {";}
            ivId = innerVre->add(i,i->ci(),false);
            break;
        }
        case Keywords::l::para_do:{
            flT = L_DO;
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_curly_open)){throw "While is missing {";}
            ivId = innerVre->add(i,i->ci(),false);   // Back to VRE
            i->operator++();
            if(!i->isK(Keywords::l::para_while)){throw "Do is missing while!";}
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_round_open)){throw "Error! While is missing '('";}
            i->operator++();
            exc = new MultiExecutor*[1];
            executerCount =1;
            exc[0] = new MultiExecutor(i,innerVre);
            exc[0]->endOfEx = Keywords::l::para_bracket_round_close;
            exc[0]->parse();
            if(!i->isK(Keywords::l::para_bracket_round_close)){throw "WHy is there no ')' after the while?";}
            i->operator++();
            break;
        }
        case Keywords::l::para_switch:{
            flT = L_SWITCH;
            if(i->ci() != 0){
                i->operator--();
                if(i->isK(Keywords::l::para_final)) this->headerIvID = SUBEXECUTOR_SWITCH_FINAL;
                i->operator++();
            }
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_round_open)){throw "Error! While is missing '('";}
            i->operator++();
            exc = new MultiExecutor*[1];
            executerCount =1;
            exc[0] = new MultiExecutor(i,innerVre);
            exc[0]->endOfEx = Keywords::l::para_bracket_round_close;
            exc[0]->parse();
            if(!i->isK(Keywords::l::para_bracket_round_close)){throw "WHy is there no ')' after the switch?";}
            i->operator++();
            if(!i->isK(Keywords::l::para_bracket_curly_open)){throw "switch is missing {";}
            ivId = innerVre->add(i,i->ci(),false);   // Back to VRE
            break;
        }
        case Keywords::l::para_case:{
            std::cout << "Found Case" << std::endl;
            flT = L_CASE;
            ITERATOR_DT start = i->ci();
            i->insertParticle(new keywordsParticle(Keywords::l::para_bracket_round_open));
            while(!i->isK(Keywords::l::para_semicolon)){
                i->operator++();
            }
            i->insertParticle(new keywordsParticle(Keywords::l::para_bracket_round_close));
            i->ci(start);
            exc = new MultiExecutor*[1];
            executerCount =1;
            exc[0] = new MultiExecutor(i,innerVre);
            exc[0]->endOfEx = Keywords::l::para_bracket_round_close; //FIND WAY TO NOT GET CASE COLLIDING WITH VAR REPLACEMENT => OR NOT
            i->operator++();
            exc[0]->parse();
            break;
        }
    }
}
void MultiExecutor::resolveExecutors()
{
    // Brackets <-> Multiexecutors && TODO Replace executor with Variable &&
    // START BRACKET RELATED (CAST, METHOD;MULTI)
    i->ci(start);
    // Flow-Control
    // TODO for the Future => Add Point also between Flowcontrols
    while(i->ci() < end){
        if(i->isFlowControlKW()){
            InExecutorVariable* rt = new InExecutorVariable(varMapping.size());
            varMapping.push_back(rt);
            ITERATOR_DT startX = i->ci();
            FlowControlExecutor * flwEx = new FlowControlExecutor(i,innerVre);
            flwEx->parse();
            flwEx->result = rt->getVariableID();
            this->internalExecutors.push_back(flwEx);
            end -= (i->ci()-(startX+1));
            i->erase(startX + 1, i->ci());
            i->ci(startX);
            i->insertParticle(rt);
            //i->ci(startX+1);
        }
        i->operator++();
        i->printc("HEY" + std::to_string(i->ci()) + " " + std::to_string(end));
    }
    i->ci(start);
    // END!
    while (i->ci() < end)
    {
        goUntil(Keywords::l::para_bracket_round_open);
        if (i->ci() >= end)
            break;
        std::cout << "Found Round Open Bracket!" << std::endl;
        i->operator--();
        if (i->ct() == PARTICLE_INNER_EXECUTOR) // ==> If method then Finished!
        {                                       // Method
            resolveMethod();
            continue;
        }
        i->operator++();
        ITERATOR_DT startY = i->ci();
        i->executor_jumpBrackets(); // Go To End
        if (i->ct() == PARTICLE_INNER_EXECUTOR)
        { // Is Cast Method
            i->operator++();
            if (i->isK(Keywords::l::para_bracket_round_open))
            { // Method after cast
                i->operator--();
                resolveMethod();
            }
            i->ci(startY);
            resolveCast();
            continue;
        }
        else if (i->isK(Keywords::l::para_bracket_round_open))
        { // MultiEx after cast
            resolveMultiEx();
            i->ci(startY);
            resolveCast();
            continue;
        }
        else
        {
            i->ci(startY);
            resolveMultiEx();
        }
    }
    // END BRACKET RELATED (CAST;METHOD;MULTI)
    // Operators
    if (i->isDebug())
        std::cout << "OPX" << std::endl;
    for (unsigned int num = 0; num < sizeof(OPERATOR_ORDER) / sizeof(Keywords::l); num++)
    {
        i->ci(start);
        while (i->ci() < end)
        {
            if (i->isK(OPERATOR_ORDER[num]))
            {
                // std::cout << "FOP: " << Keywords::m[OPERATOR_ORDER[num]] << std::endl;
                // printC();
                ITERATOR_DT operatorPos = i->ci();
                InExecutorVariable *rtValue = new InExecutorVariable(varMapping.size());
                varMapping.push_back(rtValue);
                OperatorExecutor *opx = new OperatorExecutor(i,innerVre);
                opx->parse();
                opx->result = rtValue->getVariableID();
                if (i->isDebug())
                    std::cout << "S-E " << operatorPos << " <> " << end << " <> " << i->ci() << std::endl;
                i->erase(operatorPos + (opx->param1 == 0 ? 1 : 0), i->ci() + 1 + (opx->param1 == 0 ? 1 : 0));
                end = end - (opx->param1 ==0 ? 1 : (opx->param2 == 0) ? 1 : 2);
                ITERATOR_DT itx = i->ci();
                /*i->ci(start);
                while(i->ci() < end){
                    i->operator++();
                    if (i->cp()->getType() == PARTICLE_INNER_EXECUTOR)
                    {
                        std::cout << "--> $" << std::to_string(cixi()) << std::endl;
                    }
                    else
                    {
                        i->printc("-->");
                    }
                }*/
                i->ci(itx-1);
               // i->ci(operatorPos + (opx->param1 == 0 ? 0 : 1));
                if (i->cp()->getType() == PARTICLE_INNER_EXECUTOR)
                {
                    std::cout << "HELLOX $" << std::to_string(cixi()) << std::endl;
                }
                else
                {
                    i->printc("HELLOX");
                }

                // printC();
                i->insertParticle(rtValue);
                if (i->cp()->getType() == PARTICLE_INNER_EXECUTOR)
                {
                    std::cout << "HELLOX $" << std::to_string(cixi()) << std::endl;
                }
                else
                {
                    i->printc("HELLOX");
                }
                internalExecutors.push_back(opx);
                print();
            }
            i->operator++();
        }
    }
    i->ci(start);
    this->postProcessor();
    innerResult = cixi();
    i->ci(end);
}
void MultiExecutor::postProcessor() {
    if(i->ci() != end){
        i->operator++();
        if(i->ct() == PARTICLE_INNER_EXECUTOR){
            std::cout << "[FINE] Found no DOT!" << std::endl;
            InExecutorVariable * rt = new InExecutorVariable(varMapping.size());
            varMapping.push_back(rt);
            unsigned int second = cixi();
            i->operator--();
            unsigned int first = cixi();
            i->insertParticle(rt);
            OperatorExecutor* opx = new OperatorExecutor(i,innerVre);
            opx->param1 = first;
            opx->opera = Keywords::l::para_point;
            opx->param2 = second;
            opx->result = rt->getVariableID();
            internalExecutors.push_back(opx);
        }
    }
    i->ci(start);
}
void MultiExecutor::parse()
{
    this->resolveLength();
    this->replaceVars();
    if (i->isDebug())
        print();
    this->resolveExecutors();
}
void MethodExecutor::parse()
{
    i->operator++();
    if (!(i->isK() && i->ck() == Keywords::l::para_bracket_round_open))
        throw "Method is missing a B.R.O.";
    i->operator++();
    while (!i->isK(Keywords::l::para_bracket_round_close))
    {
        MultiExecutor *m = new MultiExecutor(i,innerVre);
        m->endOfEx = Keywords::l::para_comma;
        m->parse();
        params.push_back(m);
        printC();
        if (i->isK() && i->ck() == Keywords::l::para_bracket_round_close)
            break;
        i->operator++();
    }
    i->operator++();
    if (!i->isK(Keywords::l::para_bracket_curly_open))
        return;
    ivId = innerVre->add(i,i->ci(),false);// TODO <-> Find and Resolve all anonym Methods
    i->operator++();
} // At the end of the method call
