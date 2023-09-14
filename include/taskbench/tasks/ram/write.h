/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <cstdint>

namespace taskbench::ram::write {

void sequential(int* buffer, size_t size, int value);

}  // namespace taskbench::ram::write