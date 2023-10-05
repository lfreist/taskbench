/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <missocl/opencl.h>
#include <taskbench/tasks/gpu/benchmark.h>
#include <taskbench/tasks/gpu/memory.h>
#include <taskbench/tasks/gpu/mmul.h>
#include <taskbench/tasks/gpu/synthetic.h>
#include <taskbench/utils/statistics.h>

#include <thread>

namespace taskbench::gpu {

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(seconds runtime) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "GPU Benchmarks:\n");
    std::cout << std::flush;
  }
  run_mmul(runtime);
  run_synthetic(runtime);
  run_memory(runtime);
  run_transfer_speed(runtime);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_mmul(seconds runtime) {
  try {
    utils::Timer timer;

    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Matrix Multiplication Benchmarks:\n");
      std::cout << std::flush;
    }

    {  // GPU matrix multiplication
      std::string name("Matrix Multiplication");
      _register_benchmark(0, 2 * (static_cast<size_t>(2048 * 2048) * 2048) - (2048 * 2048), name);
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
        std::cout << std::flush;
      }

      size_t size = 2048 * 2048;
      std::vector<float> mat1(size);
      std::vector<float> mat2(size);
      std::vector<float> result(size);
      std::fill_n(mat1.data(), mat1.size(), 3.563f);
      std::fill_n(mat2.data(), mat2.size(), 1.23457f);
      std::fill_n(result.data(), result.size(), 0.0f);

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        timer.start();
        mmul::matrix_multiply(mat1, mat2, result, 2048);
        auto bm_time = timer.stop();
        _add_result(name, bm_time);
        _print_runtime(_benchmark_result.at(name));
        _print_o_per_second(_benchmark_result.at(name));
        rt -= rt_timer.round();
      }
    }

    if (_verbosity != VERBOSITY::OFF) {
      std::cout << std::endl;
    }
  } catch (const mcl::OpenCLError& e) {
    fmt::print("Could not run {}. Make sure you have OpenCL runtimes installed ({}).", "Matrix Multiplication Benchmarks", e.what());
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_memory(seconds runtime) {
  try {
    utils::Timer timer;

    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Memory Benchmarks:\n");
      std::cout << std::flush;
    }

    size_t size = _array_size<float>(_buffer_size);
    std::vector<float> data(size);
    std::fill_n(data.data(), data.size(), 0.0f);

    {  // GPU memory write
      std::string name("Memory Write");
      _register_benchmark(_buffer_size, 0, name);
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\r    {:40} ", name);
        std::cout << std::flush;
      }

      auto setup = memory::setup_memory_write(data);

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        timer.start();
        setup.kernel->run();
        auto bm_time = timer.stop();
        _add_result(name, bm_time);
        _print_runtime(_benchmark_result.at(name));
        _print_gib_per_second(_benchmark_result.at(name));
        rt -= rt_timer.round();
      }
    }

    {  // GPU memory read
      std::string name("Memory Read");
      _register_benchmark(_buffer_size, 0, name);
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\n    {:40} ", name);
        std::cout << std::flush;
      }

      auto setup = memory::setup_memory_read(data);

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        timer.start();
        setup.kernel->run();
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
  } catch (const mcl::OpenCLError& e) {
    fmt::print("Could not run {}. Make sure you have OpenCL runtimes installed ({}).", "Memory Benchmarks", e.what());
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_transfer_speed(seconds runtime) {
  try {
    utils::Timer timer;

    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Transfer Benchmarks:\n");
      std::cout << std::flush;
    }

    size_t size = _array_size<float>(_buffer_size);
    std::vector<float> data(size);
    std::fill_n(data.data(), data.size(), 0.0f);

    {  // write data to OpenCL device
      std::string name("Data Transfer Speed: RAM to GPU");
      _register_benchmark(_buffer_size, 0, name);
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\r    {:40} ", name);
        std::cout << std::flush;
      }

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        auto setup = memory::setup_memory(data);
        timer.start();
        setup.buffer.write_to_device();
        auto bm_time = timer.stop();
        _add_result(name, bm_time);
        _print_runtime(_benchmark_result.at(name));
        _print_gib_per_second(_benchmark_result.at(name));
        rt -= rt_timer.round();
      }
    }

    {  // read data from OpenCL device
      std::string name("Data Transfer Speed: GPU to RAM");
      _register_benchmark(_buffer_size, 0, name);
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\n    {:40} ", name);
        std::cout << std::flush;
      }

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        auto setup = memory::setup_memory(data);
        setup.buffer.write_to_device();
        timer.start();
        setup.buffer.read_from_device();
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
  } catch (const mcl::OpenCLError& e) {
    fmt::print("Could not run {}. Make sure you have OpenCL runtimes installed ({}).", "Transfer Speed Benchmarks", e.what());
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_synthetic(seconds runtime) {
  try {
    utils::Timer timer;

    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Synthetic Benchmarks:\n");
      std::cout << std::flush;
    }

    {  // computing GPU integer ops
      std::string name("Synthetic IOPS");
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\r    {:40} ", name);
        std::cout << std::flush;
      }

      size_t size = _array_size<int>(_buffer_size);
      std::vector<int> data(size);
      std::fill_n(data.data(), data.size(), 4);
      auto setup = synthetic::create_mcl_setup<int>(data);

      _register_benchmark(0, static_cast<uint64_t>(2048ull * size), name);

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        timer.start();
        setup.kernel->run();
        auto bm_time = timer.stop();
        setup.buffer.read_from_device();
        _add_result(name, bm_time);
        _print_runtime(_benchmark_result.at(name));
        _print_o_per_second(_benchmark_result.at(name));
        rt -= rt_timer.round();
      }
    }

    {  // computing GPU float operations
      std::string name("Synthetic FLOPS");
      if (_verbosity != VERBOSITY::OFF) {
        fmt::print(fg(fmt::color::azure), "\n    {:40} ", name);
        std::cout << std::flush;
      }

      size_t size = _array_size<float>(_buffer_size);
      std::vector<float> data(size);
      std::fill_n(data.data(), data.size(), 4.465f);
      auto setup = synthetic::create_mcl_setup(data);

      _register_benchmark(0, static_cast<uint64_t>(2048ull * size), name);

      utils::Timer rt_timer;
      rt_timer.start();
      auto rt = runtime;
      while (rt.count() > 0) {
        timer.start();
        setup.kernel->run();
        auto bm_time = timer.stop();
        _add_result(name, bm_time);
        _print_runtime(_benchmark_result.at(name));
        _print_o_per_second(_benchmark_result.at(name));
        rt -= rt_timer.round();
      }
    }

    if (_verbosity != VERBOSITY::OFF) {
      std::cout << std::endl;
    }
  } catch (const mcl::OpenCLError& e) {
    fmt::print("Could not run {}. Make sure you have OpenCL runtimes installed ({}).", "Synthetic Benchmarks", e.what());
  }
}

}  // namespace taskbench::gpu