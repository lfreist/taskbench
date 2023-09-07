/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/utils/statistics.h>

#include <chrono>

namespace taskbench::utils {

template <>
double mean<std::chrono::duration<double>>(const std::vector<std::chrono::duration<double>>& data) {
  if (data.empty()) {
    return 0.0f;
  }
  return std::accumulate(data.begin(), data.end(), static_cast<std::chrono::duration<double>>(0)).count() /
         static_cast<double>(data.size());
}

template <>
double stdev<std::chrono::duration<double>>(const std::vector<std::chrono::duration<double>>& data) {
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

}  // namespace taskbench::utils