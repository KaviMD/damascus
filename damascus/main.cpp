// Metal
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

// General
#include <iostream>
#include <stdlib.h>

// Damascus Utilities
#include "util/util.hpp"

void add_arrays(const float* inA,
                const float* inB,
                float* result,
                int length)
{
    for (int index = 0; index < length ; index++)
    {
        result[index] = inA[index] + inB[index];
    }
}

#define SIZE 500000
#define MAX_NUM 1000000.0f

int main(int argc, const char * argv[]) {
//    MTL::Device* pDevice = MTL::CreateSystemDefaultDevice();
//
//    pDevice->release();
    
    float a[SIZE];
    float b[SIZE];
    float out_cpu[SIZE];
    float out_gpu[SIZE];
    
    for (int i=0; i<SIZE; i++) {
        a[i] = (float) rand() / MAX_NUM;
        b[i] = (float) rand() / MAX_NUM;
    }
    
    {
        damascus::Timer timer("CPU");
        add_arrays(a, b, out_cpu, SIZE);
        
        for (int i=0; i<3; i++) {
            DAMASCUS_DEBUG(a[i] << " + " << b[i] << " = " << out_cpu[i])
        }
    }
    
    {
        damascus::Timer timer("GPU");
        add_arrays(a, b, out_gpu, SIZE);
        
        for (int i=0; i<3; i++) {
            DAMASCUS_DEBUG(a[i] << " + " << b[i] << " = " << out_cpu[i])
        }
    }
    
    return 0;
}
