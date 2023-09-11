/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <vector>

#include <missocl/opencl.h>

namespace taskbench::gpu::synthetic {

void mem_write(mcl::Kernel& kernel);

void mem_read(mcl::Kernel& kernel);

void ops_fp(mcl::Kernel& kernel);

void ops_int(mcl::Kernel& kernel);

}