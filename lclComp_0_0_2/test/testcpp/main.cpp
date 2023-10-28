#include <iostream>
#include <string>
using namespace std;

std::string replace(std::string in,std::string search,std::string replaceStr){
	std::string rt;
	size_t searchIterator = 0;
	size_t offset = 0;
	bool startFinding = false;
	size_t startPos = 0;
	for(size_t i =0; i < in.length(); i++){
		if(search[searchIterator] == in[i + offset]){
			if(!startFinding)startPos = i;
			startFinding = true;
			searchIterator++;

			if(searchIterator == search.length()){
				rt += replaceStr;
				startFinding = false;
				searchIterator = 0;
			}
		}else{
			if(startFinding){
				startFinding = false;
				i = startPos;
			}
			rt += in[i+offset];
		}
	}
	return rt;
}

std::string replaceSTR(std::string in,std::string search,std::string replaceStr,char escapeBlockArea){
	char startBlockedArea[] = {'\"'};
	char endBlockedArea[] = {'\"'};
	std::string rt;
	size_t searchIterator = 0;
	size_t startBlockedAreaSize = (sizeof(startBlockedArea) / sizeof(char));
	size_t endBlockedAreaSize = (sizeof(endBlockedArea) / sizeof(char));
	size_t offset = 0;
	bool startFinding = false;
	bool isBlockArea = false;
	size_t startPos = 0;
	for(size_t i =0; i < in.length(); i++){
		if(isBlockArea){
			for(size_t bai = 0; bai < endBlockedAreaSize; bai++){
				if(endBlockedArea[bai] == (char) in[i+offset]){
					if(!(i == 0 || escapeBlockArea == (char) in[i+offset-1]))isBlockArea = false;
					break;
				}
			}
		}
		if(search[searchIterator] == in[i + offset] && !isBlockArea){
			if(!startFinding)startPos = i;
			startFinding = true;
			searchIterator++;

			if(searchIterator == search.length()){
				rt += replaceStr;
				startFinding = false;
				searchIterator = 0;
			}
		}else{
			if(startFinding){
				startFinding = false;
				i = startPos;
			}
			rt += in[i+offset];
		}
		if(!isBlockArea){
			for(size_t bai = 0; bai < startBlockedAreaSize; bai++){
				if(startBlockedArea[bai] == (char) in[i+offset]){
					if(!(i == 0 || escapeBlockArea == (char) in[i+offset-1]))isBlockArea = true;
					break;
				}
			}
		}
	}
	return rt;
}


int main(int argc,char** argv){
	std::string strx;
	strx += "Lorem \\\"ipsum\\\" dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
     cout << replaceSTR(strx,"ipsum","abc",'\\') << endl;



}
