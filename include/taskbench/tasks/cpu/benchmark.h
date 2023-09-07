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

#include <taskbench/utils/benchmark.h>

namespace taskbench::cpu {

class Benchmark : public utils::AbstractBenchmark {
 public:
  Benchmark() = default;
  ~Benchmark() override = default;

  void run_all(unsigned iterations) override;

  void run_aes(unsigned iterations = 1);
  void run_compression(unsigned iterations = 1);
  void run_fft(unsigned iterations = 1);
  void run_mmul(unsigned iterations = 1);
  void run_sort(unsigned iterations = 1);
  void run_synthetic(unsigned iterations = 1);
};

}