#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <cmath>
#include <ctime>
#include <cstdio>
#include <memory>
#include <string>

#if defined(_MSC_VER)
    #include <direct.h>
    #define GetCurrentDir _getcwd
#elif defined(__unix__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#else
#endif

#define LOG(format, ...) \
    do { \
        fprintf(stdout, format##"\n", __VA_ARGS__); \
        fflush(stdout); \
    } while(0); \


#endif // !_GLOBAL_H_