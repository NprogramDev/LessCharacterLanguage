#include "tokenjson.hpp"

using namespace CompileLCL;

void println(std::string strx){
    std::cout << strx << std::endl;
}

std::string CompileLCL::tokenJSON(ROOT *root) {
    json::jobject rt;
    json::jobject meta;
    meta[TOKEN_JSON_VERSION_TAG] = TOKEN_JSON_VERSION;
    rt[TOKEN_JSON_META] = meta;

    rt[TOKEN_JSON_DATA] = tokenizeVRE(*root);
    return rt;
}

json::jobject CompileLCL::tokenICO(ICO *ic) {
    json::jobject curr;
    switch ((ic)->getID()) {
        case ICOID_CLASS: {
            println("CLASS!");
            curr[TOKEN_JSON_TYPE] = TOKEN_JSON_TYPE_CLASS;
            Class &x = *(dynamic_cast<Class *>(ic));
            curr["name"] = x.name;

            std::vector<json::jobject> inherList;
            for (auto it = x.inheritance.begin(); it != x.inheritance.end(); ++it) {
                inherList.push_back(particleJson((*it)->getParticle()));
            }
            curr["inheritance"] = inherList;
            curr["body"] = tokenizeVRE(x);


            break;
        }
        case ICOID_EXE: {
            println("EXECUTOR!");
            Executor &x = *(dynamic_cast<Executor *>(ic));
            curr[TOKEN_JSON_TYPE] = TOKEN_JSON_TYPE_EXECUTOR;
            curr["multiExecutor"] = jsonMultiExecutor(x.mex);
            std::vector<json::jobject> anFcs;
            for(size_t i = 1; i < x.resolvedInnerVreLen; i++){
                json::jobject anFc;
                anFc = tokenICO(x.resolvedInnerVre[i]);
                anFcs.push_back(anFc);
            }
            curr["anonymousFunctions"] = anFcs;
            curr["gdt"] = jsonGDT(x.gdt);
            break;
        }

        case ICOID_METHOD: {
            println("METHOD!");
            Method &x = *(dynamic_cast<Method *>(ic));
            curr["name"] = x.name;
            curr[TOKEN_JSON_TYPE] = TOKEN_JSON_TYPE_METHOD;
            std::vector<std::string> annot;
            for (auto &y: x.annotation) {
                annot.push_back(y->getString());
            }
            curr["annotation"] = annot;
            /*if(!x.noVarEnv && x.callbackM != nullptr){
                curr["callbackM"] = tokenizeVRE(*x.callbackM);
            }*/
            std::vector<json::jobject> transfers;
            for (auto &y: x.transfers) {
                transfers.push_back(tokenICO(y));
            }
            curr["transfers"] = transfers;
            std::vector<json::jobject> attr;
            for (auto &y: x.attr) {
                particle *a = new keywordsParticle(y);
                attr.push_back(particleJson(a));
                delete a;
            }
            curr["attributes"] = attr;
            curr["body"] = tokenizeVRE(x);
            if (x.opOverload != Keywords::l::null) {
                particle *a = new keywordsParticle(x.opOverload);
                curr["operatorOverload"] = particleJson(a);
                delete a;
            }
            curr["gdt"] = jsonGDT(x.gdt);
            break;
        }
        case ICOID_ROOT: {
            curr[TOKEN_JSON_TYPE] = "ROOT";
            break;
        }
        case ICOID_VAR: {
            curr[TOKEN_JSON_TYPE] = TOKEN_JSON_TYPE_VARIABLE;
            println("VARIABLE!");
            Variable &x = *(dynamic_cast<Variable *>(ic));
            curr["name"] = x.name;
            std::vector<std::string> annot;
            for (auto &y: x.annotation) {
                annot.push_back(y->getString());
            }
            curr["annotation"] = annot;
            /*if(!x.noVarEnv && x.callbackM != nullptr){
                curr["callbackM"] = tokenizeVRE(*x.callbackM);
            }*/
            std::vector<json::jobject> attr;
            for (auto &y: x.attr) {
                particle *a = new keywordsParticle(y);
                attr.push_back(particleJson(a));
                delete a;
            }
            curr["attributes"] = attr;
            if(x.defaultValEx != nullptr){
                curr["initExecutors"] = tokenICO(x.defaultValEx);
            }
            curr["gdt"] = jsonGDT(x.gdt);
            println("VRE END!");
            break;
        }
        case ICOID_VRE: {
            VariableEnvironment &x = *(dynamic_cast<VariableEnvironment *>(ic));
            println("VRE!");
            curr[TOKEN_JSON_TYPE] = TOKEN_JSON_TYPE_VRE;
            curr["body"] = tokenizeVRE(x);
            break;
        }
            //
        case ICOID_ICO:
        case ICOID_ICOP:
        case ICOID_ICOT:
            break;
    }
    return curr;
}

json::jobject CompileLCL::jsonGDT(CompileLCL::GenericDT *pDt) {
    return json::jobject();
}

std::vector<json::jobject> CompileLCL::tokenizeVRE(VariableEnvironment &vre) {
    std::vector<json::jobject> rt;
    for (auto it = vre.out.begin(); it < vre.out.end(); it++) {
        rt.push_back(tokenICO(*it));
    }
    return rt;
}

json::jobject CompileLCL::jsonMultiExecutor(CompileLCL::MultiExecutor *mex) {
    json::jobject rt;
    rt["type"] = "MULTIEXECUTOR";
    std::vector<json::jobject> varMap;
    for (auto &x: mex->varMapping) {
        if (x == nullptr) continue;
        varMap.push_back(particleJson(x));
    }

    rt["varMap"] = varMap;
    std::vector<json::jobject> exec;

    for (auto &x: mex->internalExecutors) {
        exec.push_back(jsonExecutor(x));
    }
    rt["executors"] = exec;
    rt["resultIndex"] = mex->result;
    return rt;
}

json::jobject CompileLCL::jsonExecutor(CompileLCL::SubExecutor *&pExecutor) {
    switch (pExecutor->getType()) {
        case EXECUTOR_METHOD: {
            json::jobject rt;
            MethodExecutor *mth = dynamic_cast<MethodExecutor *>(pExecutor);
            rt["type"] = "METHOD";
            rt["pathName"] = mth->namePath;
            rt["anonymousFunction"] = mth->ivId;
            std::vector<json::jobject> rtx;
            for (auto &a: mth->params) {
                rtx.push_back(jsonMultiExecutor(a));
            }
            rt["params"] = rtx;
            rt["result"] = mth->result;
            return rt;
        }
        case EXECUTOR_OPERATOR: {
            json::jobject rt;
            rt["type"] = "OPERATOR";

            OperatorExecutor *opx = dynamic_cast<OperatorExecutor *>(pExecutor);
            rt["result"] = opx->result;
            rt["param1"] = opx->param1;
            rt["param2"] = opx->param2;

            keywordsParticle *kw = new keywordsParticle(opx->opera);
            rt["operator"] = particleJson(kw);
            delete kw;
            return rt;
        }
        case EXECUTOR_MULTIEXECUTOR: {

            return jsonMultiExecutor(dynamic_cast<MultiExecutor *>(pExecutor));
        }
        case EXECUTOR_FLOWCONTROL:{
            FlowControlExecutor *flx = dynamic_cast<FlowControlExecutor *>(pExecutor);
            json::jobject rt;
            rt["type"] = "FLOWCONTROL";
            switch (flx->flT) {
                case L_WHILE:{
                    rt["subtype"] = "WHILE";
                    std::vector<json::jobject> executors (flx->executerCount);
                    for(size_t i = 0; i < flx->executerCount; i++){
                        executors.at(i) = jsonMultiExecutor(flx->exc[i]);
                    }
                    rt["executors"] = executors;
                    rt["anonymousFunction"] = flx->ivId;
                    break;
                }
                case L_DO:{
                    rt["subtype"] = "DO";
                    std::vector<json::jobject> executors (flx->executerCount);
                    for(size_t i = 0; i < flx->executerCount; i++){
                        executors.at(i) = jsonMultiExecutor(flx->exc[i]);
                    }
                    rt["executors"] = executors;
                    rt["anonymousFunction"] = flx->ivId;
                    break;
                }
                case L_FOR:{
                    rt["subtype"] = "FOR";
                    rt["anonymousFunctionHeader"] = flx->headerIvID;
                    rt["anonymousFunction"] = flx->ivId;
                    break;
                }
                case L_SWITCH:
                {
                    rt["subtype"] = "SWITCH";
                    std::vector<json::jobject> executors (flx->executerCount);
                    for(size_t i = 0; i < flx->executerCount; i++){
                        executors.at(i) = jsonMultiExecutor(flx->exc[i]);
                    }
                    rt["isFinal"] = flx->headerIvID == SUBEXECUTOR_SWITCH_FINAL;
                    rt["executors"] = executors;
                    rt["anonymousFunction"] = flx->ivId;
                    break;
                }
                case L_CASE:{
                    rt["subtype"] = "CASE";
                    std::vector<json::jobject> executors (flx->executerCount);
                    for(size_t i = 0; i < flx->executerCount; i++){
                        executors.at(i) = jsonMultiExecutor(flx->exc[i]);
                    }
                    rt["executors"] = executors;
                    break;
                }

            }


            return rt;
        }
        case EXECUTOR_SUB_EXECUTOR:
            return json::jobject();
    }
    return json::jobject();
}