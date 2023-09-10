/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/tasks/gpu/benchmark.h>
#include <taskbench/tasks/gpu/mmul.h>
#include <taskbench/tasks/gpu/synthetic.h>
#include <taskbench/utils/data_generator.h>
#include <taskbench/utils/statistics.h>

#include <thread>

namespace taskbench::gpu {

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "GPU Benchmarks:\n");
    std::cout << std::flush;
  }
  run_mmul(iterations);
  run_synthetic(iterations);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_mmul(unsigned int iterations) {
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  size_t size = 2048 * 2048;
  // auto mat1 = utils::DataGenerator::vector<float>(size, 42);
  // auto mat2 = utils::DataGenerator::vector<float>(size, 16);
  std::vector<float> mat1(size);
  std::vector<float> mat2(size);
  std::vector<float> result(size);
  std::fill_n(mat1.data(), mat1.size(), 3.563f);
  std::fill_n(mat2.data(), mat2.size(), 1.23457f);
  std::fill_n(result.data(), result.size(), 0.0f);

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mmul");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    mmul::matrix_multiply(mat1, mat2, result, 2048);
    auto runtime = timer.stop();
    _add_result("mmul", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("mmul")) {
        auto& results = _benchmark_result["mmul"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mmul");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_synthetic(unsigned int iterations) {}

}  // namespace taskbench::gpu