#include "cl_host.h"

const int matrix_a_height = 2000;
const int matrix_a_width  = 2000;
const int matrix_b_width  = 500;


double cpu_result[matrix_a_height * matrix_b_width];
double gpu_result[matrix_a_height * matrix_b_width];
double a[matrix_a_height * matrix_a_width];
double b[matrix_a_width * matrix_b_width];

int main() {
    ClHost cl_host;
    char current_program_dir[250];
    GetCurrentDir(current_program_dir, 250);
    std::string current_program(current_program_dir);
    for (auto& ch : current_program)
        if (ch == '\\')
            ch = '/';
    current_program += "/cl_kernel/matrix_multi.cl";

    if (!cl_host.CreateProgram(current_program.c_str()))
        return -1;

    // Create the data to process.
    srand(0);
    for (int i = 0; i < matrix_a_height * matrix_a_width; i++) {
        a[i] = rand();
    }

    for (int i = 0; i < matrix_a_width * matrix_b_width; i++) {
        b[i] = rand();
    }

    cl_host.CreateMemoryObject(a, b, matrix_a_height, matrix_a_width, matrix_b_width);

    printf("Nomal calc:\n");
    clock_t t1 = clock();
    for (int i = 0; i < matrix_a_height; i++) 
        for (int j = 0; j < matrix_b_width; j++) {
            cpu_result[i * matrix_b_width + j] = 0;
            for (int k = 0; k < matrix_a_width; k++)
                cpu_result[i * matrix_b_width + j] += a[i * matrix_a_width + k] * b[k * matrix_b_width + j];
        }

    clock_t t2 = clock();

    cl_host.Run(matrix_a_height, matrix_a_width, matrix_b_width, gpu_result);

    clock_t t3 = clock();

    for (int i = 0; i < matrix_a_height; ++i)
        for (int j = 0; j < matrix_b_width; j++)
            if (cpu_result[i * matrix_b_width + j] != gpu_result[i * matrix_b_width + j]) {
                printf("Error: Incorrect result! %6d %6.2lf %6.2lf %6.2lf %6.2lf\n", i, a[i], b[i], cpu_result[i], gpu_result[i]);
                break;
            }

    printf("cpu time = %.8f s\n", (float)(t2 - t1) / CLOCKS_PER_SEC);
    printf("gpu time = %.8f s\n", (float)(t3 - t2) / CLOCKS_PER_SEC);
    printf("Executed program succesfully.\n");

    return 0;
}