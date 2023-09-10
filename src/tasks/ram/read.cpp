/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/ram/read.h>

#include <iostream>
#include <cstring>

namespace taskbench::ram::read {

void sequential(const char* data, size_t size) {
  auto* res = std::memchr(data, '\0', size);
  if (res != nullptr) {
    std::cerr << "Sequential RAM Read benchmark did not run properly..." << std::endl;
  }
}

}  // namespace taskbench::ram::read