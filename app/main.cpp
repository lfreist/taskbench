/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/cpu/benchmark.h>

int main(int argc, char** argv) {
  taskbench::cpu::Benchmark cpu_benchmark;
  cpu_benchmark.run_all(3);
  return 0;
}