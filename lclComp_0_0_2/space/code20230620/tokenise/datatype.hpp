#ifndef DATATYPE_HPP
#define DATATYPE_HPP
#include "tokenize.hpp"
namespace CompileLCL{
//class Datatype;
//class Tokenize;
class Datatype
{
protected:
    Tokenize *tk;

public:
    static Datatype* getDatetype(Tokenize *tk);
    static int testforMultitype(Tokenize *tk);
    Datatype(Tokenize *tk);
    void parse();
};

class SimpleDatatype : public Datatype
{
public:
    int value;
    SimpleDatatype(Tokenize *tk);
    void parse();
};

class ComplexDatatype : public Datatype
{
public:
    std::string value;
    ComplexDatatype(Tokenize *tk);
    void parse();
};

class MultiDatatype : public Datatype
{
public:
    std::vector<std::string> value;
    MultiDatatype(Tokenize *tk, int numberOfRTDTs);
    void parse();
};
}
#endif