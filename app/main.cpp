/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/taskbench.h>

int main(int argc, char** argv) {
  taskbench::cpu::Benchmark cpu_benchmark;
  taskbench::ram::Benchmark ram_benchmark;
  taskbench::gpu::Benchmark gpu_benchmark;
  cpu_benchmark.run_all(taskbench::seconds(1));
  ram_benchmark.run_all(taskbench::seconds(1));
  gpu_benchmark.run_all(taskbench::seconds(1));

  std::cout << "\nPress 'Enter' to exit ";
  std::cin.get();
  return 0;
}