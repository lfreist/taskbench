/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <string>
#include <map>

#include <taskbench/tasks/cpu/aes.h>
#include <taskbench/tasks/cpu/compression.h>
#include <taskbench/tasks/cpu/fft.h>
#include <taskbench/tasks/cpu/mmul.h>
#include <taskbench/tasks/cpu/sort.h>
#include <taskbench/tasks/cpu/synthetic.h>

#include <taskbench/benchmark.h>

namespace taskbench::cpu {

class Benchmark : public AbstractBenchmark {
 public:
  Benchmark() = default;
  ~Benchmark() override = default;

  void run_all(seconds runtime) override;

  void run_aes(seconds runtime);
  void run_compression(seconds runtime);
  void run_fft(seconds runtime);
  void run_mmul(seconds runtime);
  void run_sort(seconds runtime);
  void run_synthetic(seconds runtime);

 private:
  uint64_t _num_ops {4000000000};
  uint64_t _num_ops_div {400000000};
};

}