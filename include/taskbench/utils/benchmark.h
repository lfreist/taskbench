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

#define S_1_MiB 0x100000
#define S_2_MiB 0x200000
#define S_4_MiB 0x400000
#define S_8_MiB 0x800000
#define S_16_MiB 0x1000000
#define S_32_MiB 0x2000000
#define S_64_MiB 0x4000000
#define S_128_MiB 0x8000000
#define S_256_MiB 0x10000000
#define S_512_MiB 0x20000000
#define S_1_GiB 0x40000000
#define S_2_GiB 0x80000000
#define S_4_GiB 0x100000000
#define S_8_GiB 0x200000000

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