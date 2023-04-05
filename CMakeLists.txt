cmake_minimum_required(VERSION 3.10)
project(SelfModifyingProgram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include asmjit
add_subdirectory(external/asmjit)

add_executable(self_modify main.cpp)

target_link_libraries(self_modify PRIVATE asmjit)

