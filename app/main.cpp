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
  ram_benchmark.run_all(taskbench::seconds(3));
  cpu_benchmark.run_all(taskbench::seconds(3));
  gpu_benchmark.run_all(taskbench::seconds(3));

  std::cout << "\nPress 'Enter' to exit ";
  std::cin.get();
  return 0;
}