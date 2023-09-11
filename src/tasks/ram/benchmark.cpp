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
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  int num_threads = static_cast<int>(std::thread::hardware_concurrency());

  std::random_device dev;

  std::vector<char> data(S_1_GiB);
  std::fill_n(data.data(), data.size(), utils::DataGenerator::integer<char>(dev(), 1));

  // sequential
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "read");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  auto seq_partition_size = S_1_GiB / num_threads;
  for (unsigned i = 0; i < iterations; ++i) {
    std::vector<std::thread> threads(num_threads);
    int pos = 0;
    timer.start();
    for (auto& t : threads) {
      t = std::thread(read::sequential, data.data() + pos, seq_partition_size);
      pos += seq_partition_size;
    }
    for (auto& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    auto runtime = timer.stop();
    _add_result("read", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("read")) {
        auto& results = _benchmark_result["read"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "read");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                   1 / utils::min(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_write(unsigned int iterations) {
  utils::Timer timer;

  int num_threads = static_cast<int>(std::thread::hardware_concurrency());
  auto partition_size = S_256_MiB / num_threads;

  // multi thread
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "    {:20} ", "write");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  std::random_device dev;

  for (unsigned i = 0; i < iterations; ++i) {
    auto* data = new int[S_256_MiB];
    std::vector<std::thread> threads(num_threads);
    int pos = 0;
    auto value = utils::DataGenerator::integer<int>(dev());
    timer.start();
    for (auto& t : threads) {
      t = std::thread(write::sequential, data + pos, partition_size, value);
      pos += partition_size;
    }
    for (auto& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    auto runtime = timer.stop();
    _add_result("write", runtime);

    if (data[0] != data[partition_size + 1]) {
      std::cerr << "RAM Benchmarks failed. This line of code should never be reached and only exists to avoid the "
                   "compiler from optimizing things away...";
    }

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("write")) {
        auto& results = _benchmark_result["write"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "write");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                   static_cast<double>(sizeof(int)) / 4.0f / utils::min(results));
      }
      std::cout << std::flush;
    }
    delete[] data;
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_read_write(unsigned int iterations) {
  utils::Timer timer;

  int num_threads = static_cast<int>(std::thread::hardware_concurrency());
  auto partition_size = S_256_MiB / num_threads;

  // multi thread
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "    {:20} ", "read/write");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  auto* data = new int[S_256_MiB];
  std::random_device dev;

  for (unsigned i = 0; i < iterations; ++i) {
    auto dst = new int[S_256_MiB];
    std::memset(data, utils::DataGenerator::integer<int>(dev()), S_256_MiB);
    std::vector<std::thread> threads(num_threads);
    int pos = 0;
    timer.start();
    for (auto& t : threads) {
      t = std::thread(read_write::sequential, data + pos, dst + pos, partition_size);
      pos += partition_size;
    }
    for (auto& t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    auto runtime = timer.stop();
    _add_result("read/write", runtime);

    if (data[partition_size] != dst[partition_size]) {
      std::cerr << "RAM Benchmarks failed. This line of code should never be reached and only exists to avoid the "
                   "compiler from optimizing things away...";
    }

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("read/write")) {
        auto& results = _benchmark_result["read/write"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "read/write");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                   static_cast<double>(sizeof(int)) / 4.0f / utils::min(results));
      }
      std::cout << std::flush;
    }
    delete[] dst;
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
  delete[] data;
}

}  // namespace taskbench::ram