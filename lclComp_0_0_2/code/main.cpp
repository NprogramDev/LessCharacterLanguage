#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "simplyfy/keywords.hpp"
#include "simplyfy/simply.hpp"
#include "simplyfy/classify.hpp"
#include "helper.hpp"
#include "tokenise/buildtree.hpp"
#include "print.hpp"
#include "simplyfy/simplyJson.hpp"
#include "translation/lcx.hpp"
#include "tokenise/tokenjson.hpp"
#include "libs/betterJSON.h"
#include <ctime>
#include <sstream>

/**
 * Libary:
 * - https://github.com/gregjesl/simpleson/tree/master
*/
// #include "tokenise/icobj.hpp"
// #include "tokenise/tokenize.hpp"
// #define MAX_KEYWORD_LENGTH 20
using namespace CompileLCL;
bool isRunning = true;
int getSizeOfString(char *str)
{
	for (int i = 0; i < 100000000000; i++)
	{
		if (str[i] == '\0')
		{
			return i;
		}
	}
	return -1;
}

int number(std::string x)
{
	for (int i = 0; i < Keywords::length; i++)
	{
		std::string curr = Keywords::m[i];
		if (curr == x)
		{
			return i;
		}
	}
	return -1;
}
std::string simplyfy(std::string txt)
{
	Simply x(txt);
	x.parse();
	return x.getResult();
}
std::string readFile(std::string path)
{
	std::ifstream i;
	std::string rt;
	std::string txt;
	i.open(path);
	while (getline(i, txt))
	{
		rt += ("\n" + txt);
		txt = "";
	}
	i.close();
	return rt;
}
bool writeFile(std::string path,std::string& content){
    try{
    std::ofstream o;
    o.open(path);
    o.write(content.c_str(),content.size());
    o.close();
    return true;
    }catch (...){
        return false;
    }
}
class Attributes
{
	std::string *args;
	int argc;

public:
	Attributes(int argc, char **argv)
	{
		this->argc = argc;
		args = new std::string[argc];
		for (int i = 0; i < argc; i++)
		{
			args[i] = std::string(argv[i]);
		}
	}
	std::string findFirstValue(std::string kw)
	{
		for (int i = 0; i < argc; i++)
		{
			if (kw == args[i])
			{
				if (i + 1 >= argc)
					throw "Attributes has no value!";
				return args[i + 1];
			}
		}
		throw "No attribute could be found with the keyword: " + kw;
	}
	std::list<std::string> findAllValue(std::string kw)
	{
		std::list<std::string> rt;
		for (int i = 0; i < argc; i++)
		{
			if (kw == args[i])
			{
				if (i + 1 >= argc)
					throw "Attributes has no value!";
				rt.push_back(args[i + 1]);
			}
		}
		return rt;
	}
	void replace(std::string kw, std::string kw2)
	{
		for (int i = 0; i < argc; i++)
		{
			if (args[i] == kw)
			{
				args[i] = kw2;
			}
		}
	}
	void translate(std::string kw, std::string kw2)
	{
		replace(kw, kw2);
	}
	int doesExist(std::string kw)
	{
		for (int i = 0; i < argc; i++)
		{
			if (kw == args[i])
			{
				return i + 1;
			}
		}
		return 0;
	}
};
void cmdError(char const *error)
{
	std::string s = std::string(error) + std::string("\n If you don't know how this command works use --help or -h or /? ");
	throw s;
}


#define EXIT_CODE_HELP 300
#define EXIT_COMPILED_SUCCESS EXIT_SUCCESS
#define EXIT_COMPILED_WITH_ERRORS EXIT_FAILURE



void translateAll(Attributes &args){
	args.translate("-h", "--help");
		args.translate("/?", "--help");
		//
		args.translate("-f", "--file");
		args.translate("-fl", "--fileList");
		//
		args.translate("-so", "--simplyOutput");
		args.translate("-sd", "--simplyDebug");
		args.translate("-sp", "--simplyToConsoleWithPipe");
		args.translate("-sk", "--simplyToConsoleAsKeywords");
		args.translate("-sj", "--simplyJson");
		args.translate("-sfj", "--simplyFileJson");

		args.translate("-td", "--tokeniseDebug");
		args.translate("-ts", "--tokeniseToConsoleSimple");
		args.translate("-tj", "--tokeniseJson");
		args.translate("-tfj", "--tokeniseFileJson");
}
void printHelp(Attributes &args){
	printHeadline("HELP PAGE");
	std::cout << "lclcompiler [options]" << std::endl;
	printHeadline("OPTIONS");
	printList("-h,--help,/?", "Shows the help Page");
	printList("-f,--file", "The argument to compile a file e.g. -f a.lcl");
	printList("-fl,--fileList", "Prints a file list");

	printList("-so,--simplyOutput", "Store the output of the Simplifier in files in Binary.");
	printList("-sd,--simplyDebug", "Activate the debug of the simplifier");
	printList("-sp,--simplyToConsoleWithPipe", "Writes the output of the simplifier to the console with Pipes and @@ for context-Strings");
	printList("-sk,--simplyToConsoleAsKeywords", "Writes the output of the simplifier to the console as Keywords");
	printList("-sj, --simplyJson", "Write the output of the simplifier to the console as a json-String");
	printList("-sfj, --simplyFileJson", "Write the output of the simplifier to JSON Files");

	printList("-td,--tokeniseDebug", "Activate the debug of the Tokeniser");
	printList("-ts,--tokeniseToConsoleSimple", "Write the output of the tokeniser to the console as a simple diagram");
	printList("-tj, --tokeniseJson", "Write the output of the tokeniser to the console as a json-String");
	printList("-tfj,--tokeniseFileJson", "Write the output of the tokeniser to JSON Files");
}

std::string getTimeDif(const std::clock_t begin_time){
	long delta_time_ms = (clock() - begin_time);
		std::string s = std::to_string(delta_time_ms);
		std::string prt = "";
		for(int i = 0; i < s.length(); i++){
			if(i == s.length() -3 && !(s.length() < 4)){
				prt += '.';
			}
			prt += s.at(i);
		}
		return prt;
}
int main(int argc, char **argv)
{
	int status = EXIT_COMPILED_SUCCESS;
	try
	{
		const std::clock_t begin_time = std::clock();
		//Init the Mapping of the Keywords to String
		Keywords::reg();
		LCX::reg();
		// Move all args into the Attributes Manager
		Attributes args(argc, argv);
		// Translate all args from the short form to the long form
		translateAll(args);
		// If one arg is Help => print Help
		if (args.doesExist("--help"))
		{
			printHelp(args);
			return EXIT_CODE_HELP;
		}
		// If no file attribute exists => Throw an error, because no file is defined
		if (!args.doesExist("--file"))
			cmdError("You don't have defined any files!");
		// Get all file args to get all files
		std::list<std::string> files = args.findAllValue("--file");
		// If the fileList argument exists => print all files
		if (args.doesExist("--fileList"))
		{
			printHeadline("All Files");
			// Iterate through all Files and print them
			for (std::list<std::string>::iterator it = files.begin(); it != files.end(); ++it)
				std::cout << '\'' << *it << '\'' << std::endl;
			std::cout << headlineSTR << std::endl;
			std::cout << headlineSTR << std::endl;
		}
		// go through all files and simplyfy them
		for (std::list<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			try
			{
				std::string txt = readFile(*it);
				std::cout << "Test File Ending" << std::endl;
				if(endsWith(*it,".lcx")){
					std::cout << "Convert lcx to lcl:" << std::endl;
					txt = LCX::translate(txt);
					std::cout << txt << std::endl;
				}
				std::cout << "Compile LCL!" << std::endl;
				// Simplyfy
				std::string binarys = simplyfy(txt);
				if(args.doesExist("--simplyToConsoleWithPipe")){
					printHeadline("Binary / Piped Simply");
					printBinaryKeywords(binarys);
					std::cout << headlineSTR << std::endl;
					std::cout << headlineSTR << std::endl;
				}	
				// Particle
				particleLine particleL = classify(binarys);
				particleL.insert(particleL.begin(), new keywordsParticle(Keywords::l::para_bracket_curly_open));
				particleL.push_back(new keywordsParticle(Keywords::l::para_bracket_curly_close));
				if(args.doesExist("--simplyJson")){
					printHeadline("SIMPLY JSON");
					std::cout << simplyJSON(particleL) << std::endl;
				}
				if(args.doesExist("--simplyToConsoleAsKeywords")){
					printHeadline("Keywords Simply");
					printParticleLine(particleL);
					std::cout << headlineSTR << std::endl;
					std::cout << headlineSTR << std::endl;
				}
					
				// ROOT
				ROOT *root = BuildTree(particleL,args.doesExist("--tokeniseDebug"));
				if(args.doesExist("--tokeniseToConsoleSimple")){
					printHeadline("Tokeniser");
					printR(root, 0);
					std::cout << headlineSTR << std::endl;
					std::cout << headlineSTR << std::endl;
				}
                bool tj = args.doesExist("--tokeniseJson");
                bool tfj = args.doesExist("--tokeniseFileJson");
                bool btyf = true;
                if(tj || tfj){
                    std::string json = tokenJSON(root);
                    if(btyf){
                        json = beatiful(json);
                    }
                    if(tj){
                        std::cout << std::endl << json << std::endl;
                    }
                    if(tfj){
                        if(writeFile((*it) + ".json",json)){
                            std::cout << "[File was written correctly!]" << *it << ".json" << std::endl;
                        }else{
                            std::cout << "[File wasn't written!]" << *it << ".json" << std::endl;
                        }
                    }

                }
			}
			catch (std::string e)
			{
				status = EXIT_COMPILED_WITH_ERRORS;
				std::cout << "[ERROR]" << e << std::endl;
			}
			catch (char const *x)
			{
				status = EXIT_COMPILED_WITH_ERRORS;
				std::cout << "[ERROR]" << x << std::endl;
			}
		}
		std::string s = getTimeDif(begin_time);
		std::cout << ((status == EXIT_COMPILED_SUCCESS) ?"Compilation Successful" : "Compilation Unsuccessful") << " => Took " << s << ((s.length() < 4) ? "ms" : "s") << std::endl;
	}
	catch (std::string e)
	{
		std::cout << "[ERRGL] " << e << std::endl;
	}
	catch (char const *e)
	{
		std::cout << "[ERRGL] " << e << std::endl;
	}
	//	Logger::waitForClose();
}
