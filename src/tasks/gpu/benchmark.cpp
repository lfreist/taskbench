/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/tasks/gpu/benchmark.h>
#include <taskbench/tasks/gpu/mmul.h>
#include <taskbench/utils/data_generator.h>
#include <taskbench/utils/statistics.h>

#include <missocl/opencl.h>

#include <thread>

namespace taskbench::gpu {

KERNEL_CODE(
    read_kernel, __kernel void read_kernel(__global float* data) {
      const uint id = get_global_id(0);
      float x = 0.0f;
      for (uint i = 0u; i < 16; i++) {
        x += data[i * 16777216 + id];
      }
      data[id] = x;
    });

KERNEL_CODE(
    write_kernel, __kernel void write_kernel(__global float* data) {
      const uint id = get_global_id(0);
      for (uint i = 0u; i < 16; i++) {
        data[i * 16777216 + id] = 0.0f;
      }
    });

KERNEL_CODE(
    ops_fp_kernel, __kernel void ops_fp_kernel(__global float* data) {
      float x = (float)get_global_id(0);
      float y = (float)get_global_id(0);
      for (uint i = 0; i < 512; ++i) {
        x = fma(y, x, y);
        y = fma(x, y, x);
      }
      data[get_global_id(0)] = y;
    });

KERNEL_CODE(
    ops_int_kernel, __kernel void ops_int_kernel(__global float* data) {
      int x = get_global_id(0);
      int y = get_global_id(0);
      for (uint i = 0; i < 512; ++i) {
        x = (y * x) + y;
        y = (x * y) + x;
      }
      data[get_global_id(0)] = y;
    });

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
void Benchmark::run_synthetic(unsigned int iterations) {
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  {  // write
    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\r    {:20} ", "write");
      fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
      std::cout << std::flush;
    }

    mcl::Environment env;
    mcl::Memory<1, float> memory(&env, S_1_GiB / sizeof(float), 0.0f);
    memory.write_to_device();
    auto kernel = env.add_kernel(cl::NDRange(16777216), "write_kernel", write_kernel);
    kernel.set_parameters(memory);

    for (unsigned i = 0; i < iterations; ++i) {
      timer.start();
      kernel.run();
      auto runtime = timer.stop();
      _add_result("write", runtime);

      if (_verbosity != utils::VERBOSITY::OFF) {
        fmt::print("\r                                                             ");
        if (_benchmark_result.contains("write")) {
          auto& results = _benchmark_result["write"];
          fmt::print(fg(fmt::color::azure), "\r    {:20} ", "write");
          fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
          fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
          fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                     static_cast<double>(memory.mem_size()) / S_1_GiB / utils::min(results));
        }
        std::cout << std::flush;
      }
    }
  }

  {  // read
    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20} ", "read");
      fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
      std::cout << std::flush;
    }

    mcl::Environment env;
    mcl::Memory<1, float> memory(&env, S_1_GiB / sizeof(float), 1.43f);
    memory.write_to_device();
    auto kernel = env.add_kernel(cl::NDRange(16777216), "read_kernel", read_kernel);
    kernel.set_parameters(memory);

    for (unsigned i = 0; i < iterations; ++i) {
      timer.start();
      kernel.run();
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
                     static_cast<double>(memory.mem_size()) / S_1_GiB / utils::min(results));
        }
        std::cout << std::flush;
      }
    }
  }

  {  // to device bandwidth
    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20} ", "write to device");
      fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
      std::cout << std::flush;
    }

    for (unsigned i = 0; i < iterations; ++i) {
      mcl::Environment env;
      mcl::Memory<1, float> memory(&env, S_1_GiB / sizeof(float), 1.43f);
      timer.start();
      memory.write_to_device();
      auto runtime = timer.stop();
      _add_result("write to device", runtime);

      if (_verbosity != utils::VERBOSITY::OFF) {
        fmt::print("\r                                                             ");
        if (_benchmark_result.contains("write to device")) {
          auto& results = _benchmark_result["write to device"];
          fmt::print(fg(fmt::color::azure), "\r    {:20} ", "write to device");
          fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
          fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
          fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                     static_cast<double>(memory.mem_size()) / S_1_GiB / utils::min(results));
        }
        std::cout << std::flush;
      }
    }
  }

  {  // from device bandwidth
    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20} ", "read from device");
      fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
      std::cout << std::flush;
    }

    for (unsigned i = 0; i < iterations; ++i) {
      mcl::Environment env;
      mcl::Memory<1, float> memory(&env, S_1_GiB / sizeof(float), 1.43f);
      memory.write_to_device();
      timer.start();
      memory.read_from_device();
      auto runtime = timer.stop();
      _add_result("read from device", runtime);

      if (_verbosity != utils::VERBOSITY::OFF) {
        fmt::print("\r                                                             ");
        if (_benchmark_result.contains("read from device")) {
          auto& results = _benchmark_result["read from device"];
          fmt::print(fg(fmt::color::azure), "\r    {:20} ", "read from device");
          fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
          fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
          fmt::print(fg(fmt::color::blue_violet), "[{:.2f} GiB/s]",
                     static_cast<double>(memory.mem_size()) / S_1_GiB / utils::min(results));
        }
        std::cout << std::flush;
      }
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

}  // namespace taskbench::gpu