/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/gpu/memory.h>

namespace taskbench::gpu::memory {

std::string get_mem_read_kernel() {
  KERNEL_CODE(
      read_kernel, __kernel void read_kernel(__global float* data) {
        const uint id = get_global_id(0);
        float x = 0.0f;
        for (uint i = 0u; i < 16; i++) {
          x += data[i * 16777216 + id];
        }
        data[id] = x;
      });
  return read_kernel;
}

std::string get_mem_write_kernel() {
  KERNEL_CODE(
      write_kernel, __kernel void write_kernel(__global float* data) {
        const uint id = get_global_id(0);
        for (uint i = 0u; i < 16; i++) {
          data[i * 16777216 + id] = 0.0f;
        }
      });
  return write_kernel;
}

mclSetup<float> setup_memory_read(std::vector<float>& data) {
  mclSetup<float> setup(data);
  setup.kernel = std::make_unique<mcl::Kernel>(setup.env.add_kernel(16777216, "read_kernel", get_mem_read_kernel()));
  setup.kernel->set_parameters(setup.buffer);
  setup.buffer.write_to_device();
  return setup;
}

mclSetup<float> setup_memory_write(std::vector<float>& data) {
  mclSetup<float> setup(data);
  setup.kernel = std::make_unique<mcl::Kernel>(setup.env.add_kernel(16777216, "write_kernel", get_mem_write_kernel()));
  setup.kernel->set_parameters(setup.buffer);
  setup.buffer.write_to_device();
  return setup;
}

mclSetup<float> setup_memory(std::vector<float>& data) {
  return mclSetup<float>(data);
}

}