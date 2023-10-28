#include "logger.hpp"
using namespace CompileLCL;
bool Logger::isRunning = true;
std::thread* Logger::thr = nullptr;
std::ofstream* Logger::fos = nullptr;
void Logger::init(){
    std::filesystem::create_directories("log");
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream q;
    q << std::put_time(&tm, "%d_%m_%Y_%H_%M_%S_.log");
    fos = new std::ofstream(q.str());
    std::thread cy(tick);
    thr = &cy;
}
void Logger::waitForClose(){
    isRunning = false;
    thr->join();
}
void Logger::tick(){
    while(isRunning){
        /*std::ostringstream oss;
        std::cout.rdbuf(oss.rdbuf());
        (*fos) << oss.str();*/
    }
    fos->close();
}