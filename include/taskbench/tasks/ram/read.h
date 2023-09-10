/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/data_generator.h>

#include <cstddef>
#include <vector>

namespace taskbench::ram::read {

/**
 * @brief
 * @param fst
 * @param snd
 * @param size
 */
void sequential(const char* data, size_t size);

}  // namespace taskbench::ram::read