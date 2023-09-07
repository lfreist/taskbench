/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <random>
#include <type_traits>
#include <vector>

namespace taskbench::utils {

class DataGenerator {
 public:
  template <class T>
    requires IsInteger<T>
  static std::vector<T> vector(size_t size, int seed, T min = std::numeric_limits<T>::min(),
                               T max = std::numeric_limits<T>::max()) {
    std::vector<T> result(size);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int64_t> dist(static_cast<int64_t>(min), static_cast<int64_t>(max));
    for (auto& i : result) {
      i = static_cast<T>(dist(rng));
    }
    return result;
  }

  template <class T>
    requires IsFloatingPoint<T>
  static std::vector<T> vector(size_t size, int seed, T min = std::numeric_limits<T>::min(),
                               T max = std::numeric_limits<T>::max()) {
    std::vector<T> result(size);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<T> dist(min, max);
    for (auto& i : result) {
      i = dist(rng);
    }
    return result;
  }
};

}  // namespace taskbench::utils