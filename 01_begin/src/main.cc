#include <global.h>

int main(int argc, char **argv) {
    cl_int  CL_status       = CL_SUCCESS;
    cl_uint platform_cnt    = 0;

    CL_status = clGetPlatformIDs(0, NULL, &platform_cnt);

    cl_platform_id *platforms = new cl_platform_id[platform_cnt];
    clGetPlatformIDs(platform_cnt, platforms, 0);

    if (CL_status == CL_SUCCESS) {
        printf("%u platforms(s) found\n", platform_cnt);
        for (cl_uint i = 0; i < platform_cnt; ++i) {

            size_t size = 0;
            cl_int cl_status;

            // Discover and initialize the platforms
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, 0, &size);
            char *platform_name = new char[size];
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, size, platform_name, 0);
            printf(" [ %d ] %s\n", i, platform_name);

            // vendor
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, NULL, &size);
            char *vendor = new char[size];
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, size, vendor, NULL);
            printf(" - CL_PLATFORM_VENDOR:%s\n", vendor);

            // version
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 0, NULL, &size);
            char *version = new char[size];
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, size, version, NULL);
            printf(" - CL_PLATFORM_VERSION:%s\n", version);

            // profile
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 0, NULL, &size);
            char *profile = new char[size];
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, size, profile, NULL);
            printf(" - CL_PLATFORM_PROFILE:%s\n", profile);

            // extensions
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &size);
            char *extensions = new char[size];
            cl_status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, size, extensions, NULL);
            printf(" - CL_PLATFORM_EXTENSIONS:%s\n", extensions);
            
            delete[] platform_name;
            delete[] vendor;
            delete[] version;
            delete[] profile;
            delete[] extensions;

            cl_uint device_cnt;
            cl_status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, nullptr, &device_cnt);
            cl_device_id *devices = new cl_device_id[device_cnt];
            cl_status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, device_cnt, devices, nullptr);
            printf(" - Device List(%u):\n", device_cnt);
            for (unsigned int j = 0; j < device_cnt; ++j) {
                size_t      name_len;
                size_t      work_item_per_group_max;
                cl_uint     computer_unit_max       = 0;
                cl_ulong    global_mem_size_max     = 0;
                cl_ulong    constant_buffer_size_max= 0;
                cl_ulong    local_mem_size_max      = 0;
                char *      device_name             = nullptr;

                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &name_len);
                device_name = new char[name_len];
                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, name_len, device_name, NULL);
                printf("   - device_name: %s\n", device_name);
                delete[] device_name;

                // print parallel compute units(CU)
                clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computer_unit_max), &computer_unit_max, NULL);
                printf("   - computer_unit_max: %u\n", computer_unit_max);

                // print work_item_per_group_max
                clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(work_item_per_group_max), &work_item_per_group_max, NULL);
                printf("   - work_item_per_group_max: %zd\n", work_item_per_group_max);

                // print global_mem_size_max
                clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size_max), &global_mem_size_max, NULL);
                printf("   - global_mem_size_max: %llu(MB)\n", global_mem_size_max / 1024 / 1024);

                // print constant_buffer_size_max
                clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(constant_buffer_size_max), &constant_buffer_size_max, NULL);
                printf("   - constant_buffer_size_max: %llu(KB)\n", constant_buffer_size_max / 1024);

                // print local_mem_size_max
                clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_mem_size_max), &local_mem_size_max, NULL);
                printf("   - local_mem_size_max: %llu(KB)\n", local_mem_size_max / 1024);
            }
        }

        delete[] platforms;
    } else {
        printf("clGetPlatformIDs(%i)\n", CL_status);
    }

    return 0;
}