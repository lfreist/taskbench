/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/tasks/cpu/aes.h>
#include <taskbench/tasks/cpu/compression.h>
#include <taskbench/tasks/cpu/fft.h>
#include <taskbench/tasks/cpu/mmul.h>
#include <taskbench/tasks/cpu/sort.h>
#include <taskbench/tasks/cpu/synthetic.h>
#include <taskbench/utils/benchmark.h>

#include <map>
#include <string>

namespace taskbench::ram {

class Benchmark : public utils::AbstractBenchmark {
 public:
  Benchmark() = default;
  ~Benchmark() override = default;

  void run_all(unsigned iterations) override;

  void run_read(unsigned iterations = 1);
  void run_write(unsigned iterations = 1);
  void run_read_write(unsigned iterations = 1);
};

}  // namespace taskbench::ram