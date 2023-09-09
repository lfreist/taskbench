/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <fmt/color.h>
#include <taskbench/tasks/cpu/benchmark.h>
#include <taskbench/utils/data_generator.h>
#include <taskbench/utils/format.h>
#include <taskbench/utils/statistics.h>

namespace taskbench::cpu {

// _____________________________________________________________________________________________________________________
void Benchmark::run_all(unsigned iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::beige) | fmt::emphasis::bold, "CPU Benchmarks:\n");
    std::cout << std::flush;
  }
  run_aes(iterations);
  run_compression(iterations);
  run_fft(iterations);
  run_mmul(iterations);
  run_sort(iterations);
  run_synthetic(iterations);
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_aes(unsigned iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  AES Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }
  auto plain_data = utils::DataGenerator::vector<unsigned char>(S_16_MiB, 42, 48, 122);
  auto key = utils::DataGenerator::vector<unsigned char>(255, 16, 48, 122);
  std::vector<unsigned char> encrypted_data;
  encrypted_data.reserve(plain_data.size());

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                             ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "encryption");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // encryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    encrypted_data = aes::encrypt(plain_data, key);
    auto runtime = timer.stop();
    _add_result("AES encryption", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("AES encryption")) {
        auto& results = _benchmark_result["AES encryption"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "encryption");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "decryption");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // decryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    plain_data = aes::decrypt(encrypted_data, key);
    auto runtime = timer.stop();
    _add_result("AES decryption", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("AES decryption")) {
        auto& results = _benchmark_result["AES decryption"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "decryption");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_compression(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  ZStandard Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }
  auto plain_data = utils::DataGenerator::vector<char>(S_512_MiB, 42, 48, 122);
  std::vector<char> compressed_data(plain_data.size());

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                              ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "compression");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // encryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    compression::compress(plain_data, compressed_data);
    auto runtime = timer.stop();
    _add_result("compression", runtime);
    compressed_data.resize(plain_data.size());

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("compression")) {
        auto& results = _benchmark_result["compression"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "compression");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "decompression");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // decryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    compression::decompress(compressed_data, plain_data);
    auto runtime = timer.stop();
    _add_result("decompression", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("decompression")) {
        auto& results = _benchmark_result["decompression"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "decompression");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_fft(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  FFT Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }
  auto plain_data = utils::DataGenerator::vector<double>(S_2_MiB, 42);
  std::valarray<std::complex<double>> data(plain_data.size());
  std::transform(plain_data.begin(), plain_data.end(), begin(data), [](auto v) { return v + 1; });

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                      ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "fft");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // encryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    fft::fft(data);
    auto runtime = timer.stop();
    _add_result("fft", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("fft")) {
        auto& results = _benchmark_result["fft"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "fft");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "ifft");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // decryption
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    fft::ifft(data);
    auto runtime = timer.stop();
    _add_result("ifft", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("ifft")) {
        auto& results = _benchmark_result["ifft"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "ifft");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_mmul(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Matrix Multiplication Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
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
  for (int i = 0; i < matrix_size; ++i) {
    int_mat1[i] = utils::DataGenerator::vector<int>(matrix_size, i);
    int_mat2[i] = utils::DataGenerator::vector<int>(matrix_size, 2000 - i);
    int_result[i] = std::vector<int>(matrix_size);
    fp_mat1[i] = utils::DataGenerator::vector<double>(matrix_size, i);
    fp_mat2[i] = utils::DataGenerator::vector<double>(matrix_size, 2000 - i);
    fp_result[i] = std::vector<double>(matrix_size);
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                             ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mmul (int)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // int multiplication
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    mmul::matrix_multiply(int_mat1, int_mat2, int_result);
    auto runtime = timer.stop();
    _add_result("mmul (int)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("mmul (int)")) {
        auto& results = _benchmark_result["mmul (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mmul (int)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "mmul (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // double multiplication
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    mmul::matrix_multiply(fp_mat1, fp_mat2, fp_result);
    auto runtime = timer.stop();
    _add_result("mmul (double)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("mmul (double)")) {
        auto& results = _benchmark_result["mmul (double)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mmul (double)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_sort(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Sort Benchmarks:\n");
    std::cout << std::flush;
  }
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data: int...");
    std::cout << std::flush;
  }
  auto int_data = utils::DataGenerator::vector<int>(S_16_MiB, 42);
  std::vector<std::vector<int>> int_iter_data(iterations);
  for (auto& iter_data : int_iter_data) {
    iter_data = int_data;
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "\r    Building benchmark data: double...");
    std::cout << std::flush;
  }
  auto fp_data = utils::DataGenerator::vector<double>(S_16_MiB, 42);
  std::vector<std::vector<double>> fp_iter_data(iterations);
  for (auto& iter_data : fp_iter_data) {
    iter_data = fp_data;
  }
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "\r    Building benchmark data: string...");
    std::cout << std::flush;
  }
  auto string_data = utils::DataGenerator::vector<std::string>(S_2_MiB, 42);
  std::vector<std::vector<std::string>> string_iter_data(iterations);
  for (auto& iter_data : string_iter_data) {
    iter_data = string_data;
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                              ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "sort (int)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // sort int
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    sort::sort(int_iter_data[i]);
    auto runtime = timer.stop();
    _add_result("sort (int)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("sort (int)")) {
        auto& results = _benchmark_result["sort (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "sort (int)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "sort (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // sort double
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    sort::sort(fp_iter_data[i]);
    auto runtime = timer.stop();
    _add_result("sort (double)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("sort (double)")) {
        auto& results = _benchmark_result["sort (double)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "sort (double)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "sort (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // sort std::string
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    sort::sort(string_iter_data[i]);
    auto runtime = timer.stop();
    _add_result("sort (string)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("sort (string)")) {
        auto& results = _benchmark_result["sort (string)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "sort (string)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s", utils::mean(results), utils::stdev(results));
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
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "  Synthetic Benchmarks:\n");
    std::cout << std::flush;
  }

  size_t nop = 4000000000;
  size_t nop_div_100 = nop / 100;
  size_t nop_div = 400000000;
  size_t nop_div_div_100 = nop_div / 100;
  utils::Timer timer;

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::slate_gray) | fmt::emphasis::italic, "    Building benchmark data...");
    std::cout << std::flush;
  }
  auto int_data = utils::DataGenerator::vector<int>(10, 3, 1, 342);
  auto fp_data = utils::DataGenerator::vector<double>(10, 3, 1, 342);

  int int_threshold = std::numeric_limits<int>::max() -
                      std::accumulate(int_data.begin(), int_data.end(), 1, [](auto a, auto b) { return a * b; });

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print("\r                                             ");
    fmt::print(fg(fmt::color::azure), "\r    {:20} ", "add/sub (int)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // add/sub (int)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::add_sub(nop_div_100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                       int_data[6], int_data[7], int_data[8], int_data[9]);
    auto runtime = timer.stop();
    _add_result("add/sub (int)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("add/sub (int)")) {
        auto& results = _benchmark_result["add/sub (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "add/sub (int)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "mul (int)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // mul (int)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::mul(nop_div_100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                   int_data[6], int_data[7], int_data[8], int_data[9], int_threshold);
    auto runtime = timer.stop();
    _add_result("mul (int)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("mul (int)")) {
        auto& results = _benchmark_result["mul (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mul (int)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "div (int)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // div (int)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::div(nop_div_div_100, int_data[0], int_data[1], int_data[2], int_data[3], int_data[4], int_data[5],
                   int_data[6], int_data[7], int_data[8], int_data[9]);
    auto runtime = timer.stop();
    _add_result("div (int)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("div (int)")) {
        auto& results = _benchmark_result["div (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "div (int)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop_div) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "add/sub (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // add/sub (double)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::add_sub(nop_div_100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5], fp_data[6],
                       fp_data[7], fp_data[8], fp_data[9]);
    auto runtime = timer.stop();
    _add_result("add/sub (double)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("add/sub (double)")) {
        auto& results = _benchmark_result["add/sub (double)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "add/sub (double)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "mul (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // mul (double)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::mul(nop_div_100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5], fp_data[6],
                   fp_data[7], fp_data[8], fp_data[9], std::numeric_limits<double>::max());
    auto runtime = timer.stop();
    _add_result("mul (double)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("mul (double)")) {
        auto& results = _benchmark_result["mul (int)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "mul (double)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n    {:20} ", "div (double)");
    fmt::print(fg(fmt::color::gray), "(0/{})...", iterations);
    std::cout << std::flush;
  }

  // div (int)
  for (unsigned i = 0; i < iterations; ++i) {
    timer.start();
    synthetic::div(nop_div_div_100, fp_data[0], fp_data[1], fp_data[2], fp_data[3], fp_data[4], fp_data[5], fp_data[6],
                   fp_data[7], fp_data[8], fp_data[9]);
    auto runtime = timer.stop();
    _add_result("div (double)", runtime);

    if (_verbosity != utils::VERBOSITY::OFF) {
      fmt::print("\r                                                             ");
      if (_benchmark_result.contains("div (double)")) {
        auto& results = _benchmark_result["div (double)"];
        fmt::print(fg(fmt::color::azure), "\r    {:20} ", "div (double)");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", i + 1, iterations);
        fmt::print(fg(fmt::color::green), "({:.3f} +/- {:.3f}) s ", utils::mean(results), utils::stdev(results));
        fmt::print(fg(fmt::color::blue_violet), "[{}]",
                   utils::pretty_ops(static_cast<double>(nop_div) / utils::mean(results)));
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    std::cout << std::endl;
  }
}

}  // namespace taskbench::cpu