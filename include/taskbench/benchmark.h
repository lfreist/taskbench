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

#include <nlohmann/json.hpp>

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

namespace taskbench {

typedef std::chrono::duration<double> seconds;

class BenchmarkResult {
 public:
  BenchmarkResult(std::string name, size_t data_size, size_t num_operations);

  double runtime_mean() const;
  double runtime_stdev() const;
  double runtime_max() const;
  double runtime_min() const;

  double ops_mean() const;
  double ops_stdev() const;
  double ops_max() const;
  double ops_min() const;

  double bps_mean() const;
  double bps_stdev() const;
  double bps_max() const;
  double bps_min() const;

  const std::vector<seconds>& runtimes() const;
  void add_runtime(seconds runtime);

  nlohmann::json json();

  const std::string& name() const;

  size_t data_size() const;

 private:
  std::string _name;
  std::vector<seconds> _runtimes;
  size_t _data_size;
  size_t _num_operations;
};


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

  virtual void run_all(seconds runtime) = 0;

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

 protected:
  template <typename T>
  static size_t _array_size(size_t buffer_size) {
    return buffer_size / sizeof(T);
  }

  void _register_benchmark(size_t data_size, size_t num_operations, const std::string& name);

  void _add_result(const std::string& key, seconds val);

  void _print_runtime(const BenchmarkResult& bm_res);
  void _print_gib_per_second(const BenchmarkResult& bm_res);
  void _print_o_per_second(const BenchmarkResult& bm_res);

  std::map<std::string, BenchmarkResult> _benchmark_result;

  VERBOSITY _verbosity = VERBOSITY::DETAILED;
};


}  // namespace taskbench