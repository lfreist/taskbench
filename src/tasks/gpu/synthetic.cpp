/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/gpu/synthetic.h>

namespace taskbench::gpu::synthetic {

std::string get_int_ops_kernel() {
  KERNEL_CODE(
      ops_int_kernel, __kernel void ops_int_kernel(__global int* data) {
        int x = get_global_id(0);
        int y = get_global_id(0);
        for (uint i = 0; i < 512; ++i) {
          x = (y * x) + y;
          y = (x * y) + x;
        }
        data[get_global_id(0)] = y;
      });
  return ops_int_kernel;
}

std::string get_fp_ops_kernel() {
  KERNEL_CODE(
      ops_fp_kernel, __kernel void ops_fp_kernel(__global float* data) {
        float x = (float)get_global_id(0);
        float y = (float)get_global_id(0);
        for (uint i = 0; i < 512; ++i) {
          x = fma(y, x, y);
          y = fma(x, y, x);
        }
        data[get_global_id(0)] = y;
      });
  return ops_fp_kernel;
}

template <>
mclSetup<int> create_mcl_setup(std::vector<int>& data) {
  mclSetup<int> setup(data);
  setup.kernel =
      std::make_unique<mcl::Kernel>(setup.env.add_kernel(data.size(), "ops_int_kernel", get_int_ops_kernel()));
  setup.kernel->set_parameters(setup.buffer);
  return setup;
}

template <>
mclSetup<float> create_mcl_setup(std::vector<float>& data) {
  mclSetup<float> setup(data);
  setup.kernel = std::make_unique<mcl::Kernel>(setup.env.add_kernel(data.size(), "ops_fp_kernel", get_fp_ops_kernel()));
  setup.kernel->set_parameters(setup.buffer);
  return setup;
}

}  // namespace taskbench::gpu::synthetic