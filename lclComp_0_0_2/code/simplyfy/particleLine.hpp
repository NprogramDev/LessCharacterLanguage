#ifndef PARTICLE_LINE_HPP_
#define PARTICLE_LINE_HPP_
#include <vector>

#define PARTICLE_UNDEF 0
#define PARTICLE_NUMB 1
#define PARTICLE_CONTEXT 2
#define PARTICLE_STRING 3
#define PARTICLE_KEYWORDS 4
#define PARTICLE_INNER_EXECUTOR 5
namespace CompileLCL{
class particle{
        protected:
        int type;
        public:    
            int getType();
            particle();
            virtual ~particle() = default;
    };
typedef std::vector<particle*> particleLine;



}

#endif