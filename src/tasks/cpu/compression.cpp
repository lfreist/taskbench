/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/cpu/compression.h>
#include <taskbench/utils/data_generator.h>
#include <zstd.h>

namespace taskbench::cpu::compression {

// _____________________________________________________________________________________________________________________
void compress(const std::vector<char>& src, std::vector<char>& dst) {
  auto size = ZSTD_compress((void*)dst.data(), dst.size(), src.data(), src.size(), 9);
  dst.resize(size);
}

// _____________________________________________________________________________________________________________________
void decompress(const std::vector<char>& src, std::vector<char>& dst) {
  ZSTD_decompress((void*)dst.data(), dst.size(), src.data(), src.size());
}

}  // namespace taskbench::cpu::compression