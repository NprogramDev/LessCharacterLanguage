#include "string.hpp"

using namespace LCLS;

String::String() : std::string(){

}
String::String(const std::string& s) : std::string(s){

}
String::String(const std::string& str, size_t pos, size_t len) : std::string(str,pos,len){

}
String::String(const char* s) : std::string(s){

}
String::String(const char* s, size_t n) : std::string(s,n){

}
String::String(size_t n, char c) : std::string(n,c){

}
