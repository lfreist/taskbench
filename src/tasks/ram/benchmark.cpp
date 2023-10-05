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

template <typename T>
struct SmartBuffer {
  explicit SmartBuffer(size_t s) : size(s) { data = new T[size]; }
  ~SmartBuffer() { delete[] data; }
  void resize(size_t s) {
    delete[] data;
    data = new T[size];
    size = s;
  }
  T* data;
  size_t size;
};

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(seconds runtime) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "RAM Benchmarks:\n");
    std::cout << std::flush;
  }
  run_read(runtime);
  run_write(runtime);
  run_read_write(runtime);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_read(seconds runtime) {
  utils::Timer timer;

  {  // read
    std::string name("Read");
    _register_benchmark(static_cast<size_t>(_buffer_size), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    int num_threads = static_cast<int>(std::thread::hardware_concurrency());
    size_t size = _array_size<char>(_buffer_size);
    std::vector<char> data(size);
    std::fill_n(data.data(), data.size(), 34);
    auto seq_partition_size = static_cast<size_t>(_buffer_size / num_threads);
    std::vector<std::thread> threads(num_threads);

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      size_t pos = 0;
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
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_gib_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_write(seconds runtime) {
  utils::Timer timer;

  {  // write
    std::string name("Write");
    _register_benchmark(_buffer_size, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "    {:40}", name);
      std::cout << std::flush;
    }

    size_t size = _array_size<int>(_buffer_size);
    int num_threads = static_cast<int>(std::thread::hardware_concurrency());
    auto partition_size = size / num_threads;
    std::vector<std::thread> threads(num_threads);

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      SmartBuffer<int> data(size);
      size_t pos = 0;
      timer.start();
      for (auto& t : threads) {
        t = std::thread(write::sequential, data.data + pos, partition_size, 5);
        pos += partition_size;
      }
      for (auto& t : threads) {
        if (t.joinable()) {
          t.join();
        }
      }
      auto bm_time = timer.stop();
      _add_result(name, bm_time);

      // This statement will always be true and prevents the compiler from optimizing things away that are needed
      if (data.data[0] != data.data[partition_size + 1]) {
        std::cerr << "RAM Benchmarks failed. This line of code should never be reached and only exists to avoid the "
                     "compiler from optimizing things away...";
      }

      _print_runtime(_benchmark_result.at(name));
      _print_gib_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_read_write(seconds runtime) {
  utils::Timer timer;

  {  // multi thread
    std::string name("Mixed");
    _register_benchmark(_buffer_size, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "    {:40} ", name);
      std::cout << std::flush;
    }

    size_t size = _array_size<int>(_buffer_size);
    int num_threads = static_cast<int>(std::thread::hardware_concurrency());
    auto partition_size = size / num_threads;
    SmartBuffer<int> data(size);
    std::vector<std::thread> threads(num_threads);

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      SmartBuffer<int> dst(size);
      std::memset(data.data, 4, size);
      size_t pos = 0;
      timer.start();
      for (auto& t : threads) {
        t = std::thread(read_write::sequential, data.data + pos, dst.data + pos, partition_size);
        pos += partition_size;
      }
      for (auto& t : threads) {
        if (t.joinable()) {
          t.join();
        }
      }
      auto bm_time = timer.stop();
      _add_result(name, bm_time);

      if (data.data[partition_size] != dst.data[partition_size]) {
        std::cerr << "RAM Benchmarks failed. This line of code should never be reached and only exists to avoid the "
                     "compiler from optimizing things away...";
      }

      _print_runtime(_benchmark_result.at(name));
      _print_gib_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

}  // namespace taskbench::ram