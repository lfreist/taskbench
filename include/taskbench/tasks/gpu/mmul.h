/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <vector>

namespace taskbench::gpu::mmul {

void matrix_multiply(std::vector<float>& mat1, std::vector<float>& mat2, std::vector<float>& result, size_t size);

}  // namespace taskbench::gpu::mmul