/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/ram/read.h>
#include <taskbench/utils/data_generator.h>

#include <iostream>
#include <cstring>

namespace taskbench::ram::read {

void sequential(const char* data, size_t size) {
  auto res = std::memchr(data, '\0', size);
  if (res != nullptr) {
    std::cerr << "Sequential RAM Read Benchmark did not run properly..." << std::endl;
  }
}

void random(utils::Node<char>* start, size_t size) {
  auto* current = start;
  for (auto i = 0; i < size; ++i) {
    if (current->val == '\0') {
      std::cerr << "Sequential RAM Read Benchmark did not run properly..." << std::endl;
      return;
    }
    current = current->next;
  }
}

}  // namespace taskbench::ram::read