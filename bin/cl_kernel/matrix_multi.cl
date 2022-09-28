#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_amd_printf : enable

__kernel void matrix_multi_kernel(  __global const double *a,
                                    __global const double *b,
                                    __global double *result_matrix,
                                    int result_matrix_row, int result_matrix_col,
                                    int compute_size) {
    int row = get_global_id(0);
    int col = get_global_id(1);

    result_matrix[row * result_matrix_col + col] = 0;
    for (int i = 0; i < compute_size; i++) {
        result_matrix[row * result_matrix_col + col] +=
            a[row * compute_size + i] * b[i * result_matrix_col + col];
    }
    // printf("row = %d, col = %d: result_matrix[ %d ] = %ld \n", row, col, row * result_matrix_col + col, result_matrix[row * result_matrix_col + col]);
}