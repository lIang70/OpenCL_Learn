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

private:
    bool Init();
    void CleanUp();

public:
    ClHost();
    ~ClHost();

    void InitProgram(const char* source_file);

    void Run();
};

#endif // !_CL_HOST_H_