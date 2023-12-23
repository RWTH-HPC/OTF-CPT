add_library(LLVMFileCheck_Standalone FileCheck.cpp)

set_target_properties(LLVMFileCheck_Standalone
        PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)

target_link_libraries(LLVMFileCheck_Standalone PUBLIC LLVMSupport_Standalone LLVMADT_Standalone)
target_include_directories(LLVMFileCheck_Standalone PUBLIC include)
