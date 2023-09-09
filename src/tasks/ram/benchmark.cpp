/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/tasks/ram/benchmark.h>
#include <taskbench/tasks/ram/read.h>
#include <taskbench/tasks/ram/read_write.h>
#include <taskbench/tasks/ram/write.h>
#include <taskbench/utils/data_generator.h>
#include <taskbench/utils/statistics.h>

#include <thread>

namespace taskbench::ram {

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "RAM Benchmarks:\n");
    std::cout << std::flush;
  }
  run_read(iterations);
  run_write(iterations);
  run_read_write(iterations);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_read(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Read Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  int num_threads = 4;

  auto data = utils::DataGenerator::vector<char>(S_1_GiB, 42, 22, 120);
  size_t linked_list_size = S_1_GiB / sizeof(utils::Node<char>);

  auto x = linked_list_size / num_threads;
  std::vector<utils::Node<char>*> nodes(num_threads);

  for (auto& node : nodes) {
    node = utils::DataGenerator::linked_list<char>(linked_list_size / num_threads, 42);
  }

  // sequential
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "sequential read");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  auto q = S_1_GiB / num_threads;
  for (unsigned i = 0; i < iterations; ++i) {
    std::vector<std::thread> threads(num_threads);
    int pos = 0;
    timer.start();
    for (auto& t : threads) {
      t = std::thread(read::sequential, data.data() + pos, q);
      pos += q;
    }
    for (auto& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    auto runtime = timer.stop();
    _add_result("sequential read", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("sequential read")) {
        auto& results = _benchmark_result["sequential read"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "sequential read");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]", 1.0f / utils::mean(results));
      }
      std::cout << std::flush;
    }
  }

  // random
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "random read");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  for (unsigned i = 0; i < iterations; ++i) {
    std::vector<std::thread> threads(num_threads);
    timer.start();
    for (auto j = 0; j < num_threads; ++j) {
      threads[j] = std::thread(read::random, nodes[j], x);
    }
    for (auto& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    auto runtime = timer.stop();
    _add_result("random read", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("random read")) {
        auto& results = _benchmark_result["random read"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "random read");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]", 1.0f / utils::mean(results));
      }
      std::cout << std::flush;
    }
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_write(unsigned int iterations) {}

// _____________________________________________________________________________________________________________________
void Benchmark::run_read_write(unsigned int iterations) {}

}  // namespace taskbench::ram