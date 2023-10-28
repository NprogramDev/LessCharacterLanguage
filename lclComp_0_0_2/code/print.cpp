#include "print.hpp"
using namespace CompileLCL;

void CompileLCL::printParticleLine(particleLine p)
{
	printHeadline("Particle Line");
	for (int i = 0; i < p.size(); i++)
	{
		// std::cout << p.at(i)->getType() << std::endl;
		switch (p.at(i)->getType())
		{
		case PARTICLE_NUMB:
		{
			numberParticle *nm = dynamic_cast<numberParticle *>(p.at(i));
			bStorageU num = nm->getNumber();
			unsigned int sign = nm->getSign();
			switch (sign)
			{
			case Keywords::l::negativ_float:
			{
				double d = -(*((double *)&num));
				std::cout << d << std::endl;
				break;
			}

			case Keywords::l::positiv_float:
			{
				double d = *((double *)&num);
				std::cout << d << std::endl;
				break;
			}

			case Keywords::l::negativ_int:
			{
				long long d = -(*((long long *)&num));
				std::cout << d << std::endl;
				break;
			}

			case Keywords::l::positiv_int:
			{
				long long d = *((long long *)&num);
				std::cout << d << std::endl;
				break;
			}

			default:
				break;
			}
			break;
		}

		case PARTICLE_CONTEXT:
			std::cout << dynamic_cast<contextParticle *>(p.at(i))->getString() << std::endl;
			break;
		case PARTICLE_KEYWORDS:
		{
			keywordsParticle kw = *((keywordsParticle *)p.at(i));
			// std::cout << kw.getKW() << std::endl;
			std::cout << Keywords::m[kw.getKW()] << std::endl;
		}

		break;
		case PARTICLE_STRING:
			std::cout << dynamic_cast<stringParticle *>(p.at(i))->getString() << std::endl;
			break;
		default:
			std::cout << "Void Thing!" << std::endl;
			break;
		}
	}
	printHeadline("PARSE ROOT");
}
void CompileLCL::printBinaryStr(std::string prt)
{
	for (int i = 0; i < prt.length(); i++)
	{
		std::cout << (unsigned int)prt.at(i) << " | ";
	}
}
int CompileLCL::printBinaryKeywords(std::string binarys)
{
	bool isContext = 0;
	for (int i = 0; i < binarys.length(); i++)
	{
		unsigned int x = binarys.at(i);
		//			std::cout << (int) x << std::endl;
		if (x == Keywords::l::context_string || x == Keywords::l::actual_string)
		{
			isContext = !isContext;
			if (x == Keywords::l::context_string)
			{
				std::cout << "@@";
			}
			std::cout << " | ";
			continue;
		}
		if (x == Keywords::l::number)
		{
			std::string number = "";
			i++;
			if (i >= binarys.length())
			{
				return -123;
			}
			// WARN: I don't know why, but...
			unsigned char sign = binarys.at(i);
			//				std::cout << (int) sign << std::endl;
			i++;
			while (i < binarys.length() && !(((unsigned int)binarys.at(i)) == Keywords::l::number))
			{
				number += binarys.at(i) - Keywords::l::number - 1;
				// std::cout << "!!! NUMBER !!!" << (int) binarys.at(i) - Keywords::l::number - 1 << " | " << i << " | "  << std::endl;
				i++;
			}
			long long num = stringTNumber(128, number);
			//				std::cout << num << std::endl;
			long long *nup = &num;
			if (sign == Keywords::l::positiv_float)
			{
				double *p = (double *)nup;
				std::cout << "IS_NUMBER " << *p << " | ";
			}
			else if (sign == Keywords::l::negativ_float)
			{
				double *p = (double *)nup;
				std::cout << "IS_NUMBER " << -(*p) << " | ";
			}
			else if (sign == Keywords::l::positiv_int)
			{
				std::cout << "IS_NUMBER " << num << " | ";
			}
			else if (sign == Keywords::l::negativ_int)
			{
				std::cout << "IS_NUMBER " << -num << " | ";
			}
			else
			{
				std::cout << "NO SIGN!"
						  << "\n";
			}
		}
		if (isContext)
		{
			std::cout << (char)x;
		}
		else
		{
			std::cout << " | " << Keywords::m[x] << " | ";
		}
	}
	//	std::cout << number("CLE") << std::endl;
	// std::cout << Keywords::end_of_enum << std::endl;
	int max = Keywords::end_of_enum;
	std::string out = "";
	return 0;
}
bool showAdditionalInfo = false;
void printExecutor(MultiExecutor* mex,int depth){
	printHeadline("Executor Variables");
	for(unsigned int i = 0; i < mex->varMapping.size(); i++){
		printSpace(depth);
		std::cout << "$" << i << ":  ";
		if(i == 0){std::cout << "NULL" << std::endl; continue;}
		switch (mex->varMapping.at(i)->getType()) {
            case PARTICLE_CONTEXT:
                std::cout << ((contextParticle *) mex->varMapping.at(i))->getString();
                break;
            case PARTICLE_INNER_EXECUTOR:
                std::cout << "$" << ((InExecutorVariable *) mex->varMapping.at(i))->getVariableID();
                break;
            case PARTICLE_KEYWORDS:
                std::cout << Keywords::m[(((keywordsParticle *) mex->varMapping.at(i))->getKW())];
                break;
            case PARTICLE_NUMB:
                std::cout << ((numberParticle *) mex->varMapping.at(i))->getNumber();
                break;
            case PARTICLE_STRING:
                std::cout << ((stringParticle *) mex->varMapping.at(i))->getString();

        }
		std::cout << std::endl;
	}
	printHeadline("Executor List");
	for(unsigned int i = 0; i < mex->internalExecutors.size(); i++){
		printSpace(depth);
		SubExecutor* subEX = mex->internalExecutors.at(i);
		switch (subEX->getType())
		{
			case EXECUTOR_SUB_EXECUTOR: throw "PRINT SHOULD NOT BE SUB EXECUTOR!";
			case EXECUTOR_METHOD:{
				std::cout <<" -- MethodExecutor -- " << std::endl; 
				printSpace(depth* 2);
				std::cout << "Params:" << std::endl;
				//std::cout << subEX->getType();
				MethodExecutor* meth = (MethodExecutor*) mex->internalExecutors.at(i);
				for (unsigned int qt = 0; qt < meth->params.size(); qt++){
					printSpace(depth * 2);
					std::cout << "#" << qt << ": " ;
					printExecutor( meth->params.at(qt),depth*2);
				}
                printSpace(depth);
                std::cout << "Anonymouse Funciton: &" << meth->ivId << std::endl;
                std::cout << "InnerResult: $" << meth->result << std::endl;
				break;
			}
				
			case EXECUTOR_MULTIEXECUTOR:{
				printSpace(depth *2);
				std::cout <<" -- MultiExecutor -- " << std::endl;
				printExecutor((MultiExecutor*) mex->internalExecutors.at(i),depth *2);
				break;
			}
				
			case EXECUTOR_OPERATOR:{
				OperatorExecutor* ex = ((OperatorExecutor*) subEX);
				std::cout << "Keywords: " << Keywords::m[ex->opera] << std::endl;
				printSpace(depth);
				if(ex->param1 == 0){
					std::cout << "Param1: NULL" << std::endl;
				}else{
					std::cout << "Param1: $" << ex->param1 << std::endl;
				}
				printSpace(depth);
				if(ex->param2 == 0){
					std::cout << "Param2: NULL" << std::endl;
				}else{
					std::cout << "Param2: $" << ex->param2 << std::endl;
				}
				printSpace(depth);
				std::cout << "Result: $" << ex->result << std::endl;
				printSpace(depth);
				std::cout << "----------------" << std::endl;
				break;
				}
                case EXECUTOR_FLOWCONTROL:{
                    std::cout << "FLOW CONTROL EXECUTOR" << std::endl;
                    FlowControlExecutor* ex = ((FlowControlExecutor*) subEX);
                    switch (ex->flT) {
                        case L_WHILE:{
                            printSpace(depth);
                            std::cout << "WHILE:" << std::endl;
                            printHeadline("WHILE EXECUTOR");
                            printExecutor(ex->exc[0],depth+4);
                            printSpace(depth+2);
                            std::cout << "Anonymous Function &" << ex->ivId << std::endl;
                            printSpace(depth+2);
                            std::cout << "Inner Result $" << ex->result << std::endl;
                            break;
                        }
                        case L_DO:{
                            printSpace(depth);
                            std::cout << "DO:" << std::endl;
                            printHeadline("DO EXECUTOR");
                            printExecutor(ex->exc[0],depth+4);
                            printSpace(depth+2);
                            std::cout << "Anonymous Function &" << ex->ivId << std::endl;
                            printSpace(depth+2);
                            std::cout << "Inner Result $" << ex->result << std::endl;
                            break;
                            break;
                        };
                        case L_FOR:{
                            printSpace(depth);
                            std::cout << "FOR:" << std::endl;
                            printSpace(depth+2);
                            std::cout << "Anonymous Head &" << ex->headerIvID << std::endl;
                            printSpace(depth+2);
                            std::cout << "Anonymous Function &" << ex->ivId << std::endl;
                            printSpace(depth+2);
                            std::cout << "Inner Result $" << ex->result << std::endl;
                            break;
                            break;
                        };
                        case L_SWITCH:{
                            printSpace(depth);
                            std::cout << "SWITCH:" << std::endl;
                            printHeadline("SWITCH EXECUTOR");
                            printExecutor(ex->exc[0],depth+4);
                            printSpace(depth+2);
                            std::cout << "SWITCH IS FINAL: " << (ex->headerIvID == SUBEXECUTOR_SWITCH_FINAL ? "true" : "false") << std::endl;
                            std::cout << "Anonymous Function &" << ex->ivId << std::endl;
                            printSpace(depth+2);
                            std::cout << "Inner Result $" << ex->result << std::endl;
                            break;
                            break;
                        };
                        case L_CASE:{
                            printSpace(depth);
                            std::cout << "CASE:" << std::endl;
                            printHeadline("SWITCH EXECUTOR");
                            printExecutor(ex->exc[0],depth+4);

                            break;
                        };
                        default:{

                        }
                    }
                    break;
                }
		}
	}
	std::cout << "InnerResult: $" << mex->innerResult << std::endl;
}
void printAnonymousFunctions(InnerVRE &iv){
    printHeadline("Executor Anonymous Functions");
    size_t index =0;
    for(auto x : iv.extVec){

        std::cout <<  "&" << std::to_string(index) << " " << ((index = 0) ? "NULL" : "-->") << std::endl;
        index++;
    }
}
void printAnonymousFunctions(VariableEnvironment** vre,size_t len,int depth){
    printHeadline("Executor Anonymous Functions");
    for(size_t i= 0; i < len; i++){
        VariableEnvironment* vrcurr = vre[i];
        printSpace(depth+2);
        std::cout <<  "&" << std::to_string(i) << " " << ((i == 0) ? "NULL" : "-->") << std::endl;
        if(i == 0){continue;}
        printR(vrcurr,depth*2);
        std::cout << "END of &" << std::to_string(i) << std::endl;
    }
}
void printExecutor(Executor* exc,int depth){
	MultiExecutor* mex = exc->getMultiExecutor();
    //printAnonymousFunctions( exc->rawInnerVre);
    printAnonymousFunctions(exc->resolvedInnerVre,exc->resolvedInnerVreLen,depth);
	printExecutor(mex,depth);
}
void printAttributes(std::vector<Keywords::l> attr,int depth){
	if(!showAdditionalInfo)return;
	depth *= 4;
	printSpace(depth);
	std::cout << "-- Attributes --" << std::endl;
	for(int i= 0; i< attr.size(); i++){
		printSpace(depth);
		std::cout << Keywords::m[attr.at(i)] << std::endl;
	}
}
void printAnnotations(std::vector<contextParticle*> annot,int depth){
	if(!showAdditionalInfo)return;
	depth *= 4;
	printSpace(depth);
	std::cout << "-- Annotations --" << std::endl;
	for(int i= 0; i< annot.size(); i++){
		printSpace(depth);
		std::cout << annot.at(i)->getString() << std::endl;
	}
}
void printOPOverload(Keywords::l opv, int depth){
	if(!showAdditionalInfo)return;
	depth *= 4;
	printSpace(depth);
	std::cout << "-- OP Overload --" << std::endl;
	printSpace(depth);
	if(opv == Keywords::l::null){
		std::cout << "No Overload!" << std::endl;
	}else{
		std::cout << Keywords::m[opv] << std::endl;
	}
}
void printInheritance(std::vector<inheritor*> inheritance,int depth){
	printSpace(depth * 4);
	std::cout << "-- Inherit --" << std::endl;
	char msg[] = "Inherit ";
	for(int i = 0; i < inheritance.size(); i++){
		printSpace(depth*4);
		
		particle *p = inheritance.at(i)->getParticle();
				switch (p->getType())
				{
				case PARTICLE_CONTEXT:
					std::cout << msg << "[CURRENT-CONTEXT] " << dynamic_cast<contextParticle*>(p)->getString() << std::endl;
					break;
				case PARTICLE_KEYWORDS:
					std::cout << msg << "[CURRENT-KEYWORD] " << Keywords::m[dynamic_cast<keywordsParticle*>(p)->getKW()]<< std::endl;
					break;
				case PARTICLE_NUMB:
					std::cout << msg << "[CURRENT-NUMBER] " << dynamic_cast<numberParticle*>(p)->getNumber()<< std::endl;
					break;
				case PARTICLE_STRING:
					std::cout << msg << "[CURRENT-STRING] " << dynamic_cast<stringParticle*>(p)->getString()<< std::endl;
					break;
				}
	}
}
void printTransfers(std::vector<Variable*> transfers,int depth){
	for(int i = 0; i < transfers.size(); i++){
			printSpace(depth*4);
			Variable* vr = transfers.at(i);
			std::cout << "Transfer Variable ICO Name:" << vr->name << (vr->array == -1 ? "" : "[" + std::to_string(vr->array) + "]") << std::endl;
			printAttributes(vr->attr,depth+1);
			printAnnotations(vr->annotation,depth+1);
			if(showAdditionalInfo){printSpace((depth + 2)*4);std::cout << "-- Children --" << std::endl;}
			std::string msg = "           ";
			if(vr->defaultValEx != nullptr){
				printExecutor(vr->defaultValEx,(depth+1) *4);
			}
			/*if(vr->defaultValEx == nullptr)continue;*/
			/*for (int i = 0; i < vr->defaultValEx->ie.size(); i++)
			{
				particle *p = vr->defaultValEx->ie.at(i);
				switch (p->getType())
				{
				case PARTICLE_CONTEXT:
					std::cout << msg << "[CURRENT-CONTEXT] " << dynamic_cast<contextParticle*>(p)->getString();
					break;
				case PARTICLE_KEYWORDS:
					std::cout << msg << "[CURRENT-KEYWORD] " << Keywords::m[dynamic_cast<keywordsParticle*>(p)->getKW()];
					break;
				case PARTICLE_NUMB:
					std::cout << msg << "[CURRENT-NUMBER] " << dynamic_cast<numberParticle*>(p)->getNumber();
					break;
				case PARTICLE_STRING:
					std::cout << msg << "[CURRENT-STRING] " << dynamic_cast<stringParticle*>(p)->getString();
					break;
				}
			}*/
			std::cout << std::endl;
	}
}
void CompileLCL::printR(VariableEnvironment *r,int depth )
{
	if(depth == 0){
		printHeadline("ROOT CLASS");
	}
	for (int i = 0; i < r->out.size(); i++)
	{
		printSpace(depth*4);
		ICO *curICO = r->out.at(i);
		std::cout << std::to_string(i) << " | ";
		switch (curICO->getID())
		{
		case ICOID_CLASS:
		{
			Class *c = dynamic_cast<Class *>(curICO);
			std::cout << "CLASS ICO Name: " << c->name << std::endl;
			printAttributes(c->attr,depth+1);
			printAnnotations(c->annotation,depth+1);
			printInheritance(c->inheritance,depth+1);
			if(showAdditionalInfo){printSpace((depth + 2)*4);std::cout << "-- Children --" << std::endl;}
			printR(c,depth+2);
		}
		break;
		case ICOID_EXE:printROOTExecutor(dynamic_cast<Executor *>(curICO),depth+1);
		break;
		case ICOID_METHOD:{
			Method* m = dynamic_cast<Method*>(curICO);
			std::cout << "Method ICO Name: " << m->name << std::endl;
			printAttributes(m->attr,depth+1);
			printAnnotations(m->annotation,depth+1);
			printOPOverload(m->opOverload,depth+1);
			printTransfers(m->transfers,depth + 1);
			if(showAdditionalInfo){printSpace((depth + 2)*4);std::cout << "-- Children --" << std::endl;}
			printR(m,depth+2);
			break;}
		case ICOID_ROOT:
			std::cout << "ROOT ICO" << std::endl;
			break;
		case ICOID_VAR:{
			Variable* vr = dynamic_cast<Variable*>(curICO);
			std::cout << "Variable ICO Name:" << vr->name << std::endl;
			printAttributes(vr->attr,depth+1);
			printAnnotations(vr->annotation,depth+1);
			if(showAdditionalInfo){printSpace((depth + 2)*4);std::cout << "-- Children --" << std::endl;}
			std::string msg = "           ";
			if(vr->defaultValEx != nullptr){
				printExecutor(vr->defaultValEx,(depth+1) *4);
			}
			/*
			if(vr->defaultValEx == nullptr)continue;
			for (int i = 0; i < vr->defaultValEx->ie.size(); i++)
			{
				particle *p = vr->defaultValEx->ie.at(i);
				switch (p->getType())
				{
				case PARTICLE_CONTEXT:
					std::cout << msg << "[CURRENT-CONTEXT] " << dynamic_cast<contextParticle*>(p)->getString();
					break;
				case PARTICLE_KEYWORDS:
					std::cout << msg << "[CURRENT-KEYWORD] " << Keywords::m[dynamic_cast<keywordsParticle*>(p)->getKW()];
					break;
				case PARTICLE_NUMB:
					std::cout << msg << "[CURRENT-NUMBER] " << dynamic_cast<numberParticle*>(p)->getNumber();
					break;
				case PARTICLE_STRING:
					std::cout << msg << "[CURRENT-STRING] " << dynamic_cast<stringParticle*>(p)->getString();
					break;
				}
			}*/
			std::cout << std::endl;
			break;
		}
		case ICOID_VRE:
			std::cout << "VariableEnviroment ICO" << std::endl;
			break;
		case ICOID_ICO:
			std::cout << "ICO ICO" << std::endl;
			break;
		case ICOID_ICOP:
			std::cout << "ICOP ICO" << std::endl;
			break;
		case ICOID_ICOT:
			std::cout << "ICOT ICO" << std::endl;
			break;
		}
	}
}
void CompileLCL::printROOTExecutor(Executor* ex,int depth){
			std::cout << "EXECUTOR ICO" << std::endl;
			printExecutor(ex,depth +1);
			/*std::string msg = "           ";
			/*for (int i = 0; i < ex->ie.size(); i++)
			{
				particle *p = ex->ie.at(i);
				switch (p->getType())
				{
				case PARTICLE_CONTEXT:
					std::cout << msg << "[CURRENT-CONTEXT] " << dynamic_cast<contextParticle*>(p)->getString();
					break;
				case PARTICLE_KEYWORDS:
					std::cout << msg << "[CURRENT-KEYWORD] " << Keywords::m[dynamic_cast<keywordsParticle*>(p)->getKW()];
					break;
				case PARTICLE_NUMB:
					std::cout << msg << "[CURRENT-NUMBER] " << dynamic_cast<numberParticle*>(p)->getNumber();
					break;
				case PARTICLE_STRING:
					std::cout << msg << "[CURRENT-STRING] " << dynamic_cast<stringParticle*>(p)->getString();
					break;
				}
			}*/
			std::cout << std::endl;
}