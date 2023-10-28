#ifndef DATATYPE_HPP
#define DATATYPE_HPP
#include "tokenize.hpp"
#include "../helper.hpp"
#include <iostream>
namespace CompileLCL{
//class Datatype;
//class Tokenize;
class Datatype
{
protected:
    Tokenize *tk;
    int lastIndex;
public:
    static Datatype* getDatatype(Tokenize *tk);
    static int testforMultitype(Tokenize *tk);
    virtual void print(int death);
    Datatype(Tokenize *tk);
    virtual bool parse();
    int getLastIndex();
};

class SimpleDatatype : public Datatype
{
public:
    int value;
    SimpleDatatype(Tokenize *tk);
    bool parse();
    void print(int death);
};

class ComplexDatatype : public Datatype
{
public:
    std::string value;
    ComplexDatatype(Tokenize *tk);
    bool parse();
    void print(int death);
};

class MultiDatatype : public Datatype
{
public:
    Datatype** value;
    int number;
    MultiDatatype(Tokenize *tk, int numberOfRTDTs);
    bool parse();
    void print(int death);
};
}
#endif