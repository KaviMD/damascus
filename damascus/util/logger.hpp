#pragma once
#include <stdio.h>

#define DAMASCUS_ERROR(msg)
#define DAMASCUS_WARN(msg)
#define DAMASCUS_INFO(msg)
#define DAMASCUS_DEBUG(msg)

#ifdef DAMASCUS_DEBUG_LEVEL

    #if DAMASCUS_DEBUG_LEVEL >= 3
        #undef DAMASCUS_DEBUG
        #define DAMASCUS_DEBUG(msg) std::cout << "[DEBUG]: " << msg << std::endl;
    #endif

    #if DAMASCUS_DEBUG_LEVEL >= 2
        #undef DAMASCUS_INFO
        #define DAMASCUS_INFO(msg) std::cout << "[INFO]: " << msg << std::endl;
    #endif

    #if DAMASCUS_DEBUG_LEVEL >= 1
        #undef DAMASCUS_WARN
        #define DAMASCUS_WARN(msg) std::cout << "[WARN]: " << msg << std::endl;
    #endif

    #if DAMASCUS_DEBUG_LEVEL >= 0
        #undef DAMASCUS_ERROR
        #define DAMASCUS_ERROR(msg) std::cout << "[ERROR]: " << msg << std::endl;
    #endif

#endif
