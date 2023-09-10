/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/ram/write.h>

#include <cstring>

namespace taskbench::ram::write {

void sequential(int* buffer, size_t size, int value) { std::memset(buffer, value, size); }

}  // namespace taskbench::ram::write