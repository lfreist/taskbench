/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/tasks/gpu/synthetic.h>
#include <taskbench/utils/concepts.h>

#include <vector>

namespace taskbench::gpu::memory {

std::string get_mem_read_kernel();
std::string get_mem_write_kernel();

mclSetup<float> setup_memory_read(std::vector<float>& data);
mclSetup<float> setup_memory_write(std::vector<float>& data);
mclSetup<float> setup_memory(std::vector<float>& data);

}  // namespace taskbench::gpu::memory