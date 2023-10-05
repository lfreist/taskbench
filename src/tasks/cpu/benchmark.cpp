/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/tasks/cpu/benchmark.h>
#include <taskbench/utils/data_generator.h>
#include <taskbench/utils/statistics.h>

#include <thread>
#include <vector>

namespace taskbench::cpu {

struct thread_compression_data {
  std::thread thread;
  std::vector<char> plain_data;
  std::vector<char> compressed_data;
};

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(seconds run_time) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "CPU Benchmarks:\n");
    std::cout << std::flush;
  }
  run_aes(run_time);
  run_compression(run_time);
  run_fft(run_time);
  run_mmul(run_time);
  run_sort(run_time);
  run_synthetic(run_time);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_aes(seconds runtime) {
  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  AES Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }
  auto plain_data = utils::DataGenerator::vector<unsigned char>(S_16_MiB, 42, 48, 122);
  auto key = utils::DataGenerator::vector<unsigned char>(255, 16, 48, 122);
  std::vector<unsigned char> encrypted_data;
  encrypted_data.reserve(plain_data.size());

  {  // encryption
    std::string name("AES Encryption");
    _register_benchmark(S_16_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      encrypted_data = aes::encrypt(plain_data, key);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // decryption
    std::string name("AES Decryption");
    _register_benchmark(S_16_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40} ", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      plain_data = aes::decrypt(encrypted_data, key);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }
  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_compression(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  ZStandard Benchmarks:\n");
    std::cout << std::flush;
  }

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  auto plain_data = utils::DataGenerator::vector<char>(S_256_MiB, 42, 48, 122);
  std::vector<char> compressed_data(plain_data.size());

  {  // compression
    std::string name("Compression (ZStandard, 1 thread)");
    _register_benchmark(plain_data.size(), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                              ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      compression::compress(plain_data, compressed_data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      compressed_data.resize(plain_data.size());
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // decompression
    std::string name("Decompression (ZStandard, 1 thread)");
    _register_benchmark(compressed_data.size(), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      compression::decompress(compressed_data, plain_data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  std::vector<thread_compression_data> threads(std::thread::hardware_concurrency());
  auto per_thread_size = static_cast<ssize_t>(plain_data.size() / threads.size());
  // add thread specific data
  ssize_t offset = 0;
  for (auto& thread : threads) {
    thread.plain_data.assign(plain_data.begin() + offset, plain_data.begin() + offset + per_thread_size);
    thread.compressed_data = std::vector<char>(per_thread_size);
    offset += per_thread_size;
  }

  {  // compression multi thread
    std::string name("Compression (ZStandard)");
    _register_benchmark(plain_data.size(), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      for (auto& thread : threads) {
        thread.compressed_data.resize(thread.plain_data.size());
      }
      timer.start();
      for (auto& thread : threads) {
        thread.thread =
            std::thread(compression::compress, std::ref(thread.plain_data), std::ref(thread.compressed_data));
      }
      for (auto& thread : threads) {
        if (thread.thread.joinable()) {
          thread.thread.join();
        }
      }
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      compressed_data.resize(plain_data.size());
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // decompression multi thread
    std::string name("Decompression (ZStandard)");
    _register_benchmark(compressed_data.size(), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      for (auto& thread : threads) {
        thread.thread =
            std::thread(compression::decompress, std::ref(thread.compressed_data), std::ref(thread.plain_data));
      }
      for (auto& thread : threads) {
        if (thread.thread.joinable()) {
          thread.thread.join();
        }
      }
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_fft(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  FFT Benchmarks:\n");
    std::cout << std::flush;
  }

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  auto plain_data = utils::DataGenerator::vector<double>(S_2_MiB, 42);
  std::valarray<std::complex<double>> data(plain_data.size());
  std::transform(plain_data.begin(), plain_data.end(), begin(data), [](auto v) { return v + 1; });

  {  // FFT
    std::string name("Fast Fourier Transformation");
    _register_benchmark(S_2_MiB * sizeof(std::complex<double>), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                      ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      fft::fft(data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // Inverse FFT
    std::string name("Inverse Fast Fourier Transformation");
    _register_benchmark(S_2_MiB * sizeof(std::complex<double>), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      fft::ifft(data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_mmul(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Matrix Multiplication Benchmarks:\n");
    std::cout << std::flush;
  }

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }

  ssize_t matrix_size = 1024;

  auto matrix_0 = mmul::build_matrix(matrix_size, matrix_size, 42);
  auto matrix_1 = mmul::build_matrix(matrix_size, matrix_size, 24);

  {  // Matrix multiplication (int)
    std::string name("Matrix Multiplication");
    _register_benchmark(matrix_size * matrix_size * sizeof(double), -1, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      mmul::matrix_multiplication(matrix_0, matrix_1);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_sort(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Sort Benchmarks:\n");
    std::cout << std::flush;
  }

  {  // sort int
    std::string name("Sorting Integers");
    _register_benchmark(S_16_MiB * sizeof(int), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                              ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      auto data = utils::DataGenerator::vector<int>(S_16_MiB, static_cast<unsigned>(rt.count()));
      timer.start();
      sort::sort(data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // sort double
    std::string name("Sorting Floating Points");
    _register_benchmark(S_16_MiB * sizeof(double), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      auto data = utils::DataGenerator::vector<double>(S_16_MiB, static_cast<unsigned>(rt.count()));
      timer.start();
      sort::sort(data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // sort std::string
    std::string name("Sorting Strings");
    _register_benchmark(S_2_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      auto data = utils::DataGenerator::vector<std::string>(S_2_MiB, static_cast<unsigned>(rt.count()));
      timer.start();
      sort::sort(data);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  if (_verbosity != VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_synthetic(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Synthetic Benchmarks:\n");
    std::cout << std::flush;
  }

  auto int_data = utils::DataGenerator::vector<int>(10, 3, 1, 342);
  auto fp_data = utils::DataGenerator::vector<double>(10, 3, 1, 342);

  int int_threshold = std::numeric_limits<int>::max() -
                      std::accumulate(int_data.begin(), int_data.end(), 1, [](auto a, auto b) { return a * b; });

  {  // add/sub (int)
    std::string name("Synthetic: IOPS (add/sub)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::add_sub(_num_ops / 100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                         int_data[6], int_data[7], int_data[8], int_data[9]);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // mul (int)
    std::string name("Synthetic: IOPS (mul)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::mul(_num_ops / 100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                     int_data[6], int_data[7], int_data[8], int_data[9], int_threshold);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // div (int)
    std::string name("Synthetic: IOPS (div)");
    _register_benchmark(0, _num_ops_div, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::div(_num_ops_div / 100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                     int_data[6], int_data[7], int_data[8], int_data[9]);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // add/sub (double)
    std::string name("Synthetic: FLOPS (add/sub)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::add_sub(_num_ops / 100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5],
                         fp_data[6], fp_data[7], fp_data[8], fp_data[9]);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // mul (double)
    std::string name("Synthetic: FLOPS (mul)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::mul(_num_ops / 100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5], fp_data[6],
                     fp_data[7], fp_data[8], fp_data[9], std::numeric_limits<double>::max());
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // div (int)
    std::string name("Synthetic: FLOPS (div)");
    _register_benchmark(0, _num_ops_div, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:40}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      synthetic::div(_num_ops_div / 100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5],
                     fp_data[6], fp_data[7], fp_data[8], fp_data[9]);
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
}

}  // namespace taskbench::cpu