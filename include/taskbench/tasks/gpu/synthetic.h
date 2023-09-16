/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <missocl/opencl.h>

#include <memory>

template <typename T>
struct mclSetup {
  explicit mclSetup(std::vector<T>& data) : buffer(&env, data.data(), data.size()) { buffer.write_to_device(); }
  mcl::Environment env;
  mcl::Memory<1, T> buffer;
  std::unique_ptr<mcl::Kernel> kernel{nullptr};
};

namespace taskbench::gpu::synthetic {

std::string get_int_ops_kernel();
std::string get_fp_ops_kernel();

template <typename T>
mclSetup<T> create_mcl_setup(std::vector<T>& data);

template <>
mclSetup<int> create_mcl_setup(std::vector<int>& data);

template <>
mclSetup<float> create_mcl_setup(std::vector<float>& data);

}  // namespace taskbench::gpu::synthetic