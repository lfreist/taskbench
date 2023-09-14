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

#include <missocl/opencl.h>

#include <map>
#include <string>

namespace taskbench::gpu {

class Benchmark : public AbstractBenchmark {
 public:
  Benchmark() = default;
  ~Benchmark() override = default;

  void run_all(seconds runtime) override;

  void run_mmul(seconds runtime);
  void run_memory(seconds runtime);
  void run_transfer_speed(seconds runtime);
  void run_synthetic(seconds runtime);

 private:
  uint64_t _buffer_size {S_512_MiB};
};

}  // namespace taskbench::gpu