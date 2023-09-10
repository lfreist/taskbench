/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/cpu/benchmark.h>
#include <taskbench/tasks/gpu/benchmark.h>
#include <taskbench/tasks/ram/benchmark.h>

int main(int argc, char** argv) {
  taskbench::cpu::Benchmark cpu_benchmark;
  taskbench::ram::Benchmark ram_benchmark;
  taskbench::gpu::Benchmark gpu_benchmark;
  // ram_benchmark.run_all(30);
  // cpu_benchmark.run_all(3);
  gpu_benchmark.run_all(3);
  return 0;
}