#include "tokenize.hpp"
namespace CompileLCL{
class GetterMethod {
    private:
        Tokenize* tk;
    public:
        GetterMethod();
        GetterMethod(Tokenize* tk);
        void parse();
}
}
