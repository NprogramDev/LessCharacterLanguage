#ifndef LOGGER_HPP_
#define LOGGER_HPP_
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <sstream>
namespace CompileLCL{
    namespace Logger{
        extern bool isRunning;
        extern std::thread* thr;
        extern std::ofstream* fos;
        void init();
        void tick();
        void waitForClose();
    }
}

#endif