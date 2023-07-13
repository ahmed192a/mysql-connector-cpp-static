cmake_minimum_required(VERSION 3.14)

project(DB)
# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# prevent linker warnings 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

# Import mysql static lib from folder ./mysqlconncpp-static/lib64
link_directories(${PROJECT_SOURCE_DIR}/mysqlconncpp-static/lib64)

include_directories(${PROJECT_SOURCE_DIR}/mysqlconncpp-static/include)

add_executable(DB ds.cpp)

target_link_libraries(DB -static mysqlcppconn8-static z ssl crypto resolv pthread dl)
