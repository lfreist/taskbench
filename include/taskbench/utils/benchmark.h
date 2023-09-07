/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <taskbench/utils/timer.h>

namespace taskbench::utils {

typedef std::vector<std::chrono::duration<double>> BenchmarkResult;

enum class VERBOSITY {
  OFF,
  MEDIUM,
  DETAILED,
  HIGH
};

/**
 * @brief Abstract Benchmark class implemented for each task set (cpu, gpu, ram, ...)
 */
class AbstractBenchmark {
 public:
  AbstractBenchmark() = default;
  virtual ~AbstractBenchmark() = default;

  virtual void run_all(unsigned iterations) = 0;

  /**
   * @brief Delete collected benchmark results
   */
  void reset();

  /**
   * @brief Get the collected results as constant reference
   * @return
   */
  [[nodiscard]] const std::map<std::string, BenchmarkResult>& results() const;

  /**
   * @brief Get the collected results
   * @return
   */
  std::map<std::string, BenchmarkResult> results();

  /**
   * @brief Write the results to the provided stream
   * @param stream
   */
  void print() const;

 protected:
  void _add_result(const std::string& key, std::chrono::duration<double> value);

  std::map<std::string, BenchmarkResult> _benchmark_result;

  VERBOSITY _verbosity = VERBOSITY::DETAILED;
};


}  // namespace taskbench::utils