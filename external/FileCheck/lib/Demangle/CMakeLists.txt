add_library(LLVMDemangle_Standalone
        Demangle.cpp
        ItaniumDemangle.cpp
        MicrosoftDemangle.cpp
        MicrosoftDemangleNodes.cpp
        RustDemangle.cpp
        DLangDemangle.cpp
        )

target_include_directories(LLVMDemangle_Standalone PUBLIC include)
