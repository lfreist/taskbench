/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <vector>

namespace taskbench::cpu::compression {

/**
 * @brief Compress src using ZStandard
 * @param src
 * @param dst
 */
void compress(const std::vector<char>& src, std::vector<char>& dst);

/**
 * @brief Decompress ZStandard compressed src
 * @param src
 * @param dst
 */
void decompress(const std::vector<char>& src, std::vector<char>& dst);

}  // namespace taskbench::cpu::compression