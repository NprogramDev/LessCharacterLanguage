cmake_minimum_required(VERSION 3.0)
project(YourProjectName)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
file(GLOB_RECURSE SOURCE_FILES code/*.cpp code/*.cc code/*.cxx code/*.c code/*.h code/*.hpp )

add_executable(exc ${SOURCE_FILES})

