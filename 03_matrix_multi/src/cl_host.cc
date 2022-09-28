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
    context_ = clCreateContext(nullptr, device_cnt, devices, nullptr, nullptr, &CL_status);
    if (CL_status != CL_SUCCESS) {
        LOG("ERROR: Failed to create context!");
        return false;
    }

    // Selects the first of the available devices to construct the command queue.
    command_queue_ = clCreateCommandQueueWithProperties(context_, devices[0], 0, nullptr);
    device_ = devices[0];
    delete[] devices;

    return true;
}

void
ClHost::CleanUp() {
    ClearMemoryObject();
    ClearProgram();

    if (command_queue_)
        clReleaseCommandQueue(command_queue_);

    if (context_)
        clReleaseContext(context_);

    if (device_)
        clReleaseDevice(device_);

    clUnloadPlatformCompiler(paltform_);
}

ClHost::ClHost() {
    if (!Init()) {
        LOG("ERROR: Failed to initialization!");
    }
}

ClHost::~ClHost() {
    CleanUp();
}

bool 
ClHost::CreateProgram(const char* source_file) {
    FILE *kernel_file;
    fopen_s(&kernel_file, source_file, "rb");
    if (!kernel_file) {
        LOG("Couldn't find the program file");
        exit(1);
    }

    fseek(kernel_file, 0, SEEK_END);
    size_t program_size = ftell(kernel_file);
    rewind(kernel_file);
    char *src_str = new char[program_size + 1];
    src_str[program_size] = '\0';
    fread(src_str, sizeof(char), program_size, kernel_file);
    fclose(kernel_file);

    program_ = clCreateProgramWithSource(context_, 1, (const char**)&src_str, nullptr, nullptr);

    cl_int CL_status = clBuildProgram(program_, 0, nullptr, nullptr, nullptr, nullptr);
    if (CL_status == CL_BUILD_PROGRAM_FAILURE) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program_, device_, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char* error_log = new char[log_size];

        // Get the log
        clGetProgramBuildInfo(program_, device_, CL_PROGRAM_BUILD_LOG, log_size, error_log, NULL);

        // Print the log
        LOG("error: %s \n", error_log);
        delete[] error_log;
        delete[] src_str;
        return false;
    } else if (CL_status != CL_SUCCESS) {
        LOG("Failed to build program.");
        delete[] src_str;
        return false;
    }

    // Create the OpenCL kernel and allocate memory space.
    kernel_ = clCreateKernel(program_, "matrix_multi_kernel", &CL_status);
    if (CL_status != CL_SUCCESS) {
        LOG("Failed to create kernel.");
        delete[] src_str;
        return false;
    }

    delete[] src_str;
    return true;
}


void 
ClHost::ClearProgram() {
    if (kernel_)
        clReleaseKernel(kernel_);

    if (program_)
        clReleaseProgram(program_);

}

void 
ClHost::CreateMemoryObject(double *a, double *b, int matrix_a_height, int matrix_a_width, int matrix_b_width) {
    mem_objects_[0] = clCreateBuffer(context_, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   sizeof(double) * matrix_a_height * matrix_a_width, a, NULL);
    mem_objects_[1] = clCreateBuffer(context_, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   sizeof(double) * matrix_a_width * matrix_b_width, b, NULL);
    mem_objects_[2] = clCreateBuffer(context_, CL_MEM_READ_WRITE,
                                   sizeof(double) * matrix_a_height * matrix_b_width, NULL, NULL);
}

void 
ClHost::ClearMemoryObject() {
    for (int i = 0; i < 3; i++) {
        if (mem_objects_[i] != 0)
            clReleaseMemObject(mem_objects_[i]);
    }
}

void 
ClHost::Run(size_t matrix_a_height, size_t matrix_a_width, size_t matrix_b_width, void *result) {
    // Set the kernel data and execute the kernel
    cl_int CL_status;
    CL_status =  clSetKernelArg(kernel_, 0, sizeof(cl_mem), &mem_objects_[0]);
    CL_status |= clSetKernelArg(kernel_, 1, sizeof(cl_mem), &mem_objects_[1]);
    CL_status |= clSetKernelArg(kernel_, 2, sizeof(cl_mem), &mem_objects_[2]);
    CL_status |= clSetKernelArg(kernel_, 3, sizeof(int), &matrix_a_height);
    CL_status |= clSetKernelArg(kernel_, 4, sizeof(int), &matrix_b_width);
    CL_status |= clSetKernelArg(kernel_, 5, sizeof(int), &matrix_a_width);

    size_t global_work_size[2] = { matrix_a_height, matrix_b_width };

    CL_status = clEnqueueNDRangeKernel( command_queue_, kernel_, 2, NULL,
                                        global_work_size, NULL,
                                        0, NULL, NULL);

    // Fetch execution result
    CL_status = clEnqueueReadBuffer(command_queue_, mem_objects_[2], CL_TRUE,
                                    0, matrix_a_height * matrix_b_width * sizeof(double), result,
                                    0, NULL, NULL);
}