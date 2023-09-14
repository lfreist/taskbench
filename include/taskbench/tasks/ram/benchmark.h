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
#include <taskbench/benchmark.h>

#include <map>
#include <string>

namespace taskbench::ram {

class Benchmark : public AbstractBenchmark {
 public:
  Benchmark() = default;
  ~Benchmark() override = default;

  void run_all(seconds runtime) override;

  void run_read(seconds runtime);
  void run_write(seconds runtime);
  void run_read_write(seconds runtime);

 private:
  size_t _buffer_size = S_1_GiB;
};

}  // namespace taskbench::ram