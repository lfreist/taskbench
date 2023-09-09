/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <algorithm>
#include <vector>

namespace taskbench::cpu::sort {

template <typename T>
  requires utils::IsSortable<T>
void sort(std::vector<T>& data) {
  std::sort(data.begin(), data.end());
}

}  // namespace taskbench::cpu::sort