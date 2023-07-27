cmake_minimum_required(VERSION 3.14)

project(DB)
# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# use clang++ as compiler
set(CMAKE_CXX_COMPILER "clang++")
# set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
# set C++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set C++ extensions required
set(CMAKE_CXX_EXTENSIONS OFF)

# Import mysql static lib from folder ./mysqlconncpp-static/lib64
link_directories(${PROJECT_SOURCE_DIR}/mysqlconncpp-static/lib64)

include_directories(${PROJECT_SOURCE_DIR}/mysqlconncpp-static/include)

add_executable(DB db.cpp ./lib/test.cpp ./lib/test.h)

target_link_libraries(DB  -static mysqlcppconn8-static z ssl crypto resolv pthread dl)
