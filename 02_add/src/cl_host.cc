#include "cl_host.h"

bool
ClHost::Init() {
    LOG("Initializing OpenCL\n");
    
    cl_int  CL_status;

    // Get the OpenCL platform.
    CL_status = clGetPlatformIDs(1, &paltform_, 0);
    if (CL_status != CL_SUCCESS) {
        LOG("ERROR: Unable to find any OpenCL platform");
        return false;
    }

    // Access a device
    cl_uint device_cnt = 0;
    CL_status = clGetDeviceIDs(paltform_, CL_DEVICE_TYPE_GPU, 0, nullptr, &device_cnt);
    if (CL_status != CL_SUCCESS) {
        LOG("ERROR: Unable to find any GPU OpenCL device");
        return false;
    }
    cl_device_id* devices = new cl_device_id[device_cnt];
    CL_status = clGetDeviceIDs(paltform_, CL_DEVICE_TYPE_GPU, device_cnt, devices, nullptr);
    if (CL_status != CL_SUCCESS) {
        LOG("ERROR: Unable to find any GPU OpenCL device");
        return false;
    }

    // Create the context.
    context_ = clCreateContext(NULL, device_cnt, devices, NULL, NULL, &CL_status);
    if (CL_status != CL_SUCCESS) {
        LOG("ERROR: Failed to create context!");
        return false;
    }

    // Selects the first of the available devices to construct the command queue.
    command_queue_ = clCreateCommandQueue(context_, devices[0], 0, nullptr);
    device_ = devices[0];
    delete[] devices;

    return true;
}

void
ClHost::CleanUp() {
    if (kernel_)
        clReleaseKernel(kernel_);

    if (program_)
        clReleaseProgram(program_);

    if (command_queue_)
        clReleaseCommandQueue(command_queue_);

    if (context_)
        clReleaseContext(context_);

    if (device_)
        clReleaseDevice(device_);

    if (paltform_)
        delete[] paltform_;
}

ClHost::ClHost() {
    if (Init()) {
        LOG("ERROR: Failed to initialization!");
    }
}

ClHost::~ClHost() {
    CleanUp();
}

void 
ClHost::InitProgram(const char* source_file) {
    
}