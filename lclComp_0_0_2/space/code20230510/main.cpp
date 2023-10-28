#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include "keywords.hpp"
#include "simply.hpp"
//#define MAX_KEYWORD_LENGTH 20
using namespace CompileLCL;
int getSizeOfString(char * str){
	for(int i = 0;i < 100000000000;i++){
		if(str[i] == '\0'){return i;}
	}
	return -1;
}

int number(std::string x){
for(int i = 0; i < Keywords::length; i++){
	std::string curr = Keywords::m[i];
	if(curr == x){
		return i;
	}
}
return -1;
}
std::string simplyfy(std::string txt){
	Simply x(txt);
	x.parse();
	return x.getResult();
}
std::string readFile(std::string path){
	std::ifstream i;
	std::string rt;
	std::string txt;
	i.open(path);
	while(getline(i,txt)){
		rt += ("\n" + txt);
		txt = "";
	}
	i.close();
	return rt;
}
int main(int argc, char** argv){
	Keywords::reg();
	//
	std::string args[argc];
	for(int i = 0; i < argc; i++){
		args[i] = std::string(argv[i]);
		std::cout << args[i] << std::endl;
	}
	for(int i = 1; i < argc; i++){
//		std::string binaryString = simplyfy(readFile(args[i]));
		std::string txt = readFile(args[i]);
		std::cout << txt << std::endl;
		std::string binarys = simplyfy(txt);
//		std::cout << binarys << std::endl;
		bool isContext = 0;
		for(int i = 0; i < binarys.length();i++){
			std::cout << (unsigned int) binarys.at(i) << " | ";
		}
		for(int i = 0; i < binarys.length(); i++){
			unsigned int x = binarys.at(i);
//			std::cout << (int) x << std::endl;
			if(x == Keywords::l::context_string || x == Keywords::l::actual_string){
				isContext = !isContext;
				std::cout << std::endl;
				continue;
			}
			if(isContext){
				std::cout <<(char) x ;
			}else{
				std::cout << std::endl << Keywords::m[x] << std::endl;
			}
		}
	}
//	std::cout << number("CLE") << std::endl;
//std::cout << Keywords::end_of_enum << std::endl;
int max = Keywords::end_of_enum;
std::string out = "";
}
