
cmake_minimum_required(VERSION 3.13.4...3.27.4)
project(FileCheck
        VERSION 15.0.6
        DESCRIPTION "A standalone build of LLVM FileCheck")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Naming it just FileCheck leads to collisions with the LLVM cmake module
add_executable(FileCheck_Standalone FileCheck.cpp)
set_target_properties(FileCheck_Standalone
        PROPERTIES
        OUTPUT_NAME FileCheck
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)

target_include_directories(FileCheck_Standalone PRIVATE include)
target_link_libraries(FileCheck_Standalone PRIVATE LLVMFileCheck_Standalone)

add_subdirectory(lib)
