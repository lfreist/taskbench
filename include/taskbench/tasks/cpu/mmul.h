/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <concepts>
#include <cstdint>
#include <vector>

namespace taskbench::cpu::mmul {

template <typename T>
  requires utils::IsInteger<T> || utils::IsFloatingPoint<T>
void matrix_multiply(const std::vector<std::vector<T>>& mat1, const std::vector<std::vector<T>>& mat2,
                     std::vector<std::vector<T>>& result) {
  for (size_t i = 0; i < mat1.size(); ++i) {
    for (size_t j = 0; j < mat2[0].size(); ++j) {
      for (size_t k = 0; k < mat1[0].size(); ++k) {
        result[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }
}

}  // namespace taskbench::cpu::mmul