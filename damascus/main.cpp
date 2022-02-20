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

#define ARRAY_LEN 32
#define MAX_NUM 1000000.0f

int main(int argc, const char * argv[]) {
    float a[ARRAY_LEN];
    float b[ARRAY_LEN];
    float out_cpu[ARRAY_LEN];
    
    for (int i=0; i<ARRAY_LEN; i++) {
        a[i] = (float) rand() / MAX_NUM;
        b[i] = (float) rand() / MAX_NUM;
    }
    
    {
        damascus::Timer timer("CPU");
        add_arrays(a, b, out_cpu, ARRAY_LEN);
    }
    // Print out CPU Results
    for (int i=0; i<3; i++) {
        DAMASCUS_DEBUG(a[i] << " + " << b[i] << " = " << out_cpu[i])
    }
    
    // Get the default metal device and load the metal library compiled by Xcode
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    MTL::Library* defaultLibrary = device->newDefaultLibrary();
    
    if (!defaultLibrary) {
        DAMASCUS_ERROR("Failed to find the default Metal library");
        std::exit(-1);
    }
    
    // Get a reference the specific function we need out of the library
    const NS::String* functionName = NS::String::string("add_arrays", NS::ASCIIStringEncoding);
    const MTL::Function* addFunction = defaultLibrary->newFunction(functionName);
    
    if (!addFunction) {
        DAMASCUS_ERROR("Failed to find Metal function 'add_arrays'");
        std::exit(-1);
    }
    
    // Create a pipeline state oject for the adder function
    // This finishes compiling the shader for the GPU and is fairly time consuming so should only be done once
    NS::Error* error;
    const MTL::ComputePipelineState* addFunctionPSO = device->newComputePipelineState(addFunction, &error);
    
    MTL::CommandQueue* commandQueue = device->newCommandQueue();
    
    // Create the buffers add_arrays uses for CPU<->GPU communication
    MTL::ResourceOptions resourceOptions = MTL::ResourceStorageModeShared;
    MTL::Buffer* bufferA = device->newBuffer(ARRAY_LEN, resourceOptions);
    MTL::Buffer* bufferB = device->newBuffer(ARRAY_LEN, resourceOptions);
    MTL::Buffer* bufferResult = device->newBuffer(ARRAY_LEN, resourceOptions);
    
    // Fill the input buffers with the previously randomly generated data
    float* bufferAPtr = (float*) bufferA->contents();
    memcpy(bufferAPtr, a, sizeof(a));
    float* bufferBPtr = (float*) bufferB->contents();
    memcpy(bufferBPtr, b, sizeof(b));
    
    MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    MTL::ComputeCommandEncoder* computeEncoder = commandBuffer->computeCommandEncoder();
    
    // Setup the command encoder with everything it needs to run add_arrays
    computeEncoder->setComputePipelineState(addFunctionPSO);
    computeEncoder->setBuffer(bufferA, 0, 0);
    computeEncoder->setBuffer(bufferB, 0, 1);
    computeEncoder->setBuffer(bufferResult, 0, 2);
    
    // Create an ARRAY_LEN x 1 x 1 group of threads to be passed to the GPU
    MTL::Size gridSize = MTL::Size(ARRAY_LEN, 1, 1);
    
    NS::UInteger threadGroupSize = addFunctionPSO->maxTotalThreadsPerThreadgroup();
    if (threadGroupSize > ARRAY_LEN) {
        threadGroupSize = ARRAY_LEN;
    }
    // FIXME: This is a terrible variable name lol
    MTL::Size threadgroupSize = MTL::Size(threadGroupSize, 1, 1);

    // Add all the threads to the compute encoder (this does not send them to the GPU yet)
    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    
    // Tell the compute encoder that we are done adding commands to the compute pass
    computeEncoder->endEncoding();
    
    {
        damascus::Timer timer("GPU");
        
        // Commit the command buffer and execute its commands
        commandBuffer->commit();
        
        // Wait for completion (this can be replaced with addCompletedHandler for async code)
        commandBuffer->waitUntilCompleted();
    }
    
    // Copy the computed data back into our local buffer
    float* out_gpu = (float*) bufferResult->contents();
    
    // Print out CPU Results
    for (int i=0; i<3; i++) {
        DAMASCUS_DEBUG(a[i] << " + " << b[i] << " = " << out_gpu[i])
    }
    
    device->release();
    
    return 0;
}
