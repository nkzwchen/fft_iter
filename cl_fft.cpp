#include <stdio.h>
#include <stdlib.h>

#include "fft.h"
#include <Cl/cl.h>

void cl_fft_iteration(int Nr, int P, int T, complex_t* input){
    
    // init arguments
    size_t global_size[2];
    size_t local_size[2];
    global_size[0] = T;
    global_size[1] = P * Nr;  
    local_size[0] = T;
    local_size[1] = P * Nr;
    
    // init program
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_queue queue;
    cl_program program;
    cl_event event;
    cl_gpu_init(platform, device, context, queue, program, "fft_iter.cl")
    // create kernel
    const char *kernel_name_fft_iter = "fft_iter";
    cl_kernel kernel_fft_iter =
        clCreateKernel(program, kernel_fft_iter, &err);

    // allocate memory in GPU
    cl_mem X_mem =
            clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                        sizeof(complex_t) * global_size[0] * global_size[1],  input, nullptr);
    cl_mem Y_mem = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,
                        sizeof(complex_t) * global_size[0] * global_size[1], nullptr, nullptr);
    cl_mem S_mem = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,
                        sizeof(complex_t) * local_size[0] * local_size[1], nullptr, nullptr);
    // set kernel arg
    clSetKernelArg(kernel_fft_iter, 0, sizeof(cl_int), (void *)&Nr);
    clSetKernelArg(kernel_fft_iter, 1, sizeof(cl_int), (void *)&P);
    clSetKernelArg(kernel_fft_iter, 2, sizeof(cl_int), (void *)&T);
    clSetKernelArg(kernel_fft_iter, 3, sizeof(cl_mem),
                                (void *)&X_mem);
    clSetKernelArg(kernel_fft_iter, 3, sizeof(cl_mem),
                                (void *)&S_mem);

    // run kernel

    clEnqueueNDRangeKernel(queue, kernel_gemm_micro, 2, nullptr,
                           global_size, local_size, 0, nullptr,
                           &event);
    clWaitForEvents(1, &event);

    // read buffer
    clEnqueueReadBuffer(queue, Y_mem, CL_TRUE, 0,
                        sizeof(complex_t) * global_size[0] * global_size[1], X, 0, NULL,
                        &event);
    clWaitForEvents(1, &event);
}
