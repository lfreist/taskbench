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