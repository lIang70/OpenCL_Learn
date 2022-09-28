#include "cl_host.h"

const int ARRAY_SIZE = 10000000;
float    cpu_result[ARRAY_SIZE];
float    gpu_result[ARRAY_SIZE];
float    a[ARRAY_SIZE];
float    b[ARRAY_SIZE];

int main() {
    ClHost cl_host;
    char current_program_dir[250];
    GetCurrentDir(current_program_dir, 250);
    std::string current_program(current_program_dir);
    for (auto& ch : current_program)
        if (ch == '\\')
            ch = '/';
    current_program += "/cl_kernel/vector_add.cl";

    if (!cl_host.CreateProgram(current_program.c_str()))
        return -1;

    // Create the data to process.
    srand(0);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = (float)rand();
        b[i] = (float)rand();
    }

    printf("Nomal calc:\n");
    clock_t t1 = clock();
    for (int j = 0; j < ARRAY_SIZE; j++) {
        cpu_result[j] = a[j] + b[j];
    }

    clock_t t2 = clock();

    cl_host.CreateMemoryObject(a, b, ARRAY_SIZE);
    cl_host.Run(ARRAY_SIZE, gpu_result);

    clock_t t3 = clock();

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        if (std::abs(cpu_result[i] - gpu_result[i]) > (1e-2)) {
            printf("Error: Incorrect result! %d %lf %lf %lf %lf\n", i, a[i], b[i], cpu_result[i], gpu_result[i]);
            break;
        }
    }

    printf("cpu time = %.8f s\n", (float)(t2 - t1) / CLOCKS_PER_SEC);
    printf("gpu time = %.8f s\n", (float)(t3 - t2) / CLOCKS_PER_SEC);
    printf("Executed program succesfully.\n");

    return 0;
}