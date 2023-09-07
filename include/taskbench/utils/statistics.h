/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <vector>
#include <numeric>
#include <cmath>

#include <taskbench/utils/concepts.h>

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
double mean(const std::vector<std::chrono::duration<double>>& data) {
  if (data.empty()) {
    return 0.0f;
  }
  return std::accumulate(data.begin(), data.end(), static_cast<std::chrono::duration<double>>(0)).count() / static_cast<double>(data.size());
}

template <>
double stdev(const std::vector<std::chrono::duration<double>>& data) {
  if (data.empty()) {
    return 0.0f;
  }
  double meanValue = mean(data);

  double sumSquaredDifferences = 0.0;
  for (const auto& value : data) {
    double difference = value.count() - meanValue;
    sumSquaredDifferences += difference * difference;
  }
  double meanSquaredDifferences = sumSquaredDifferences / static_cast<double>(data.size());
  return std::sqrt(meanSquaredDifferences);
}

}