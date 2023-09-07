/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/utils/benchmark.h>
#include <taskbench/utils/statistics.h>

namespace taskbench::utils {

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::reset() { _benchmark_result.clear(); }

// _____________________________________________________________________________________________________________________
std::map<std::string, BenchmarkResult> AbstractBenchmark::results() { return _benchmark_result; }

// _____________________________________________________________________________________________________________________
const std::map<std::string, BenchmarkResult>& AbstractBenchmark::results() const { return _benchmark_result; }

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::print() const {
  for (auto& [name, results] : _benchmark_result) {
    if (results.size() >= 2) {
      fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::bold,
                 "{:30} (iterations: {:5}): ({:.{}f} +/- {:.{}f}) seconds\n", name, results.size(),
                 utils::mean(results), 4, utils::stdev(results), 4);
    } else {
      fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::bold,
                 "{:30} (iterations: {:5}): {:.{}f} seconds\n", name, results.size(),
                 utils::mean(results), 4);
    }
  }
}

// _____________________________________________________________________________________________________________________
void AbstractBenchmark::_add_result(const std::string& key, std::chrono::duration<double> value) {
  if (!_benchmark_result.contains(key)) {
    _benchmark_result[key] = BenchmarkResult();
  }
  _benchmark_result[key].emplace_back(value);
}

}  // namespace taskbench::utils