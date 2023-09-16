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

namespace taskbench::cpu {

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
    std::string name("AES encryption");
    _register_benchmark(S_16_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
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
    std::string name("AES decryption");
    _register_benchmark(S_16_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20} ", name);
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

  auto plain_data = utils::DataGenerator::vector<char>(S_512_MiB, 42, 48, 122);
  std::vector<char> compressed_data(plain_data.size());

  {  // encryption
    std::string name("ZSTD compression");
    _register_benchmark(S_512_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                              ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
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
    std::string name("ZSTD decompression");
    _register_benchmark(S_512_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("FFT");
    _register_benchmark(S_2_MiB * sizeof(std::complex<double>), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                      ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
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
    std::string name("Inverse FFT");
    _register_benchmark(S_2_MiB * sizeof(std::complex<double>), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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

  size_t matrix_size = 1024;
  std::vector<std::vector<int>> int_mat1(matrix_size);
  std::vector<std::vector<int>> int_mat2(matrix_size);
  std::vector<std::vector<int>> int_result(matrix_size);
  std::vector<std::vector<double>> fp_mat1(matrix_size);
  std::vector<std::vector<double>> fp_mat2(matrix_size);
  std::vector<std::vector<double>> fp_result(matrix_size);
  for (size_t i = 0; i < matrix_size; ++i) {
    int_mat1[i] = utils::DataGenerator::vector<int>(matrix_size, i);
    int_mat2[i] = utils::DataGenerator::vector<int>(matrix_size, 2000 - i);
    int_result[i] = std::vector<int>(matrix_size);
    fp_mat1[i] = utils::DataGenerator::vector<double>(matrix_size, i);
    fp_mat2[i] = utils::DataGenerator::vector<double>(matrix_size, 2000 - i);
    fp_result[i] = std::vector<double>(matrix_size);
  }

  {  // Matrix multiplication (int)
    std::string name("mmul (int)");
    _register_benchmark(1024 * 1024 * sizeof(int), 2 * static_cast<size_t>(1024 * 1024 * 1024) - (1024 * 1024), name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      mmul::matrix_multiply(int_mat1, int_mat2, int_result);
      auto bm_time = timer.stop();
      _add_result(name, bm_time);
      _print_runtime(_benchmark_result.at(name));
      _print_o_per_second(_benchmark_result.at(name));
      rt -= rt_timer.round();
    }
  }

  {  // Matrix multiplication (double)
    std::string name("mmul (double)");
    _register_benchmark(1024 * 1024 * sizeof(double), 2 * static_cast<size_t>(1024 * 1024 * 1024) - (1024 * 1024),
                        name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
      std::cout << std::flush;
    }

    utils::Timer rt_timer;
    rt_timer.start();
    auto rt = runtime;
    while (rt.count() > 0) {
      timer.start();
      mmul::matrix_multiply(fp_mat1, fp_mat2, fp_result);
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

// _____________________________________________________________________________________________________________________
void Benchmark::run_sort(seconds runtime) {
  utils::Timer timer;

  if (_verbosity != VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Sort Benchmarks:\n");
    std::cout << std::flush;
  }

  {  // sort int
    std::string name("sort (int)");
    _register_benchmark(S_16_MiB * sizeof(int), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                              ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
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
    std::string name("sort (double)");
    _register_benchmark(S_16_MiB * sizeof(double), 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("sort (string)");
    _register_benchmark(S_2_MiB, 0, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("add/sub (int)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print("\r                                             ");
      fmt::print(fg(fmt::color::azure), "\r    {:20}", name);
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
    std::string name("mul (int)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("div (int)");
    _register_benchmark(0, _num_ops_div, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("add/sub (double)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("mul (double)");
    _register_benchmark(0, _num_ops, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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
    std::string name("div (double)");
    _register_benchmark(0, _num_ops_div, name);
    if (_verbosity != VERBOSITY::OFF) {
      fmt::print(fg(fmt::color::azure), "\n    {:20}", name);
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