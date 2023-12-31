/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <numeric>
#include <vector>

namespace taskbench::utils {

template <typename T>
double mean(const std::vector<T>& data) {
  if (data.empty()) {
    return 0.0f;
  }
  return std::accumulate(data.begin(), data.end(), static_cast<T>(0)) / static_cast<double>(data.size());
}

template <typename T>
double stdev(const std::vector<T>& data) {
  double meanValue = mean(data);

  double sumSquaredDifferences = 0.0;
  for (const T& value : data) {
    double difference = static_cast<double>(value) - meanValue;
    sumSquaredDifferences += difference * difference;
  }
  double meanSquaredDifferences = sumSquaredDifferences / data.size();
  return std::sqrt(meanSquaredDifferences);
}

template <>
double mean(const std::vector<std::chrono::duration<double>>& data);

template <>
double stdev(const std::vector<std::chrono::duration<double>>& data);

template <typename T>
T max(const std::vector<T>& data) {
  return *std::max_element(data.begin(), data.end());
}

template <typename T>
T min(const std::vector<T>& data) {
  return *std::max_element(data.begin(), data.end());
}

template <typename T>
T max(const std::vector<std::chrono::duration<T>>& data) {
  return std::max_element(data.begin(), data.end())->count();
}

template <typename T>
T min(const std::vector<std::chrono::duration<T>>& data) {
  return std::min_element(data.begin(), data.end())->count();
}

}  // namespace taskbench::utils