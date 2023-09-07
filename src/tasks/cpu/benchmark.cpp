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
void Benchmark::run_all(unsigned iterations) {
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
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "AES Benchmarks:\n");
  }
  utils::Timer timer;

  auto plain_data = utils::DataGenerator::vector<unsigned char>(0x1000000, 42, 48, 122);
  auto key = utils::DataGenerator::vector<unsigned char>(0xff, 16, 48, 122);
  std::vector<unsigned char> encrypted_data;
  encrypted_data.reserve(plain_data.size());

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "  {:20} ", "encryption");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["AES encryption"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "encryption");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n  {:20} ", "decryption");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["AES decryption"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "decryption");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_compression(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "ZStandard Benchmarks:\n");
  }
  utils::Timer timer;

  auto plain_data = utils::DataGenerator::vector<char>(0x20000000, 42, 48, 122);
  std::vector<char> compressed_data(plain_data.size());

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "  {:20} ", "compression");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["compression"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "compression");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n  {:20} ", "decompression");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["decompression"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "decompression");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_fft(unsigned int iterations) {
  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::aqua) | fmt::emphasis::bold, "FFT Benchmarks:\n");
  }
  utils::Timer timer;

  auto plain_data = utils::DataGenerator::vector<double>(0x100000, 42);
  std::valarray<std::complex<double>> data(plain_data.size());
  std::transform(plain_data.begin(), plain_data.end(), begin(data), [](auto v) { return v + 1; });

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "  {:20} ", "fft");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["fft"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "fft");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }

  if (_verbosity != utils::VERBOSITY::OFF) {
    fmt::print(fg(fmt::color::azure), "\n  {:20} ", "ifft");
    fmt::print(fg(fmt::color::gray), "(1/{})...", iterations);
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
        unsigned index = i < iterations - 1 ? i + 2 : i + 1;
        auto& results = _benchmark_result["ifft"];
        fmt::print(fg(fmt::color::azure), "\r  {:20} ", "ifft");
        fmt::print(fg(fmt::color::gray), "({}/{}): ", index, iterations);
        fmt::print(fg(fmt::color::green), "({:.{}} +/- {:.{}}) s", utils::mean(results), 4, utils::stdev(results), 4);
      }
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;
}

// _____________________________________________________________________________________________________________________
void Benchmark::run_mmul(unsigned int iterations) {}

// _____________________________________________________________________________________________________________________
void Benchmark::run_sort(unsigned int iterations) {}

// _____________________________________________________________________________________________________________________
void Benchmark::run_synthetic(unsigned int iterations) {}

}  // namespace taskbench::cpu