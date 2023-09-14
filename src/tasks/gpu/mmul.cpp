/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <missocl/opencl.h>
#include <taskbench/tasks/gpu/mmul.h>

namespace taskbench::gpu::mmul {

KERNEL_CODE(
    mmul, __kernel void mmul(__global const float* A, __global const float* B, __global float* C, const int A_n,
                             const int A_m, const int B_n, const int B_m) {
      if (A_n != B_m) {
        return;
      }
      int row = get_global_id(0);
      int col = get_global_id(1);
      float sum = 0.0f;
      for (int k = 0; k < A_m; ++k) {
        sum += A[row * A_m + k] * B[k * B_m + col];
      }
      C[row * B_m + col] = sum;
    });

void matrix_multiply(std::vector<float>& mat1, std::vector<float>& mat2, std::vector<float>& result, size_t size) {
  mcl::Environment env;
  mcl::Memory<2, float> A(&env, mat1.data(), size, size);
  mcl::Memory<2, float> B(&env, mat2.data(), size, size);
  mcl::Memory<2, float> C(&env, result.data(), size, size);

  auto kernel = env.add_kernel(cl::NDRange(size, size), "mmul", mmul);

  kernel.set_parameters<mcl::Memory<2, float>>(A, B, C);  // pointer arguments
  kernel.set_args(static_cast<int>(size), static_cast<int>(size), static_cast<int>(size),
                  static_cast<int>(size));                // integer arguments
  A.write_to_device();
  B.write_to_device();

  kernel.run();

  C.read_from_device();
}

}  // namespace taskbench::gpu::mmul