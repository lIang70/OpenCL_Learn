#ifndef _CL_HOST_H_
#define _CL_HOST_H_

#include <global.h>

class ClHost {

    // OpenCL runtime configuration
    cl_platform_id      paltform_       = nullptr;
    cl_device_id        device_         = nullptr;
    cl_context          context_        = nullptr;
    cl_command_queue    command_queue_  = nullptr;

    // CL program object
    cl_program          program_        = nullptr;
    cl_kernel           kernel_         = nullptr;
    cl_mem              mem_objects_[3] = { 0, 0, 0 };

private:
    bool Init();
    void CleanUp();

public:
    ClHost();
    ~ClHost();

    bool CreateProgram(const char* source_file);
    void ClearProgram();

    void CreateMemoryObject(float *a, float *b, int size);
    void ClearMemoryObject();

    void Run(size_t size, void *result);
};

#endif // !_CL_HOST_H_