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

template <typename T>
struct Node {
  explicit Node(T value) : val(value) {}
  Node() = default;
  T val;
  Node<T>* next{nullptr};
};

class DataGenerator {
 public:
  template <typename T>
    requires IsInteger<T>
  static T integer(int seed, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int64_t> dist(static_cast<int64_t>(min), static_cast<int64_t>(max));
    return static_cast<T>(dist(rng));
  }

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

  template <typename T>
    requires std::is_same_v<T, std::string>
  static std::vector<std::string> vector(size_t size, int seed, int min = 1, int max = 256) {
    std::vector<std::string> result(size);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> size_dist(min, max);

    for (size_t i = 0; i < size; ++i) {
      result[i] = DataGenerator::string(size_dist(rng), static_cast<int>(i));
    }
    return result;
  }

  static std::string string(size_t size, int seed) {
    std::string result;
    result.resize(size);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(48, 122);

    for (size_t i = 0; i < size; ++i) {
      result[i] = static_cast<char>(dist(rng));
    }
    return result;
  }

  template <typename T>
  requires IsInteger<T>
  static Node<T>* linked_list(size_t size, int seed) {
    auto val = DataGenerator::integer<T>(seed, 48, 122);
    auto* start = new Node<T>(val);
    auto* current = start;
    for (auto i = 0; i < size; ++i) {
      auto* next = new Node<T>(val);
      current->next = next;
      current = next;
    }
    return start;
  }
};

}  // namespace taskbench::utils