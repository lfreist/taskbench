/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <cstddef>

namespace taskbench::ram::read_write {

void sequential(const int* src, int* dst, size_t size);

}  // namespace taskbench::ram::read_write