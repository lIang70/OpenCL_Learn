#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <cstdio>
#include <memory>

#define LOG(format, ...) \
    do { \
        fprintf(stdout, format##"\n", __VA_ARGS__); \
        fflush(stdout); \
    } while(0); \


#endif // !_GLOBAL_H_