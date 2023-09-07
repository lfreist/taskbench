/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/cpu/fft.h>

namespace taskbench::cpu::fft {

void fft(std::valarray<std::complex<double>>& data) {
  const size_t n = data.size();
  if (n <= 1) return;

  std::valarray<std::complex<double>> even = data[std::slice(0, n / 2, 2)];
  std::valarray<std::complex<double>> odd = data[std::slice(1, n / 2, 2)];

  fft(even);
  fft(odd);

  for (size_t k = 0; k < n / 2; ++k) {
    std::complex<double> t = std::polar(1.0, -2 * M_PI * k / n) * odd[k];
    data[k] = even[k] + t;
    data[k + n / 2] = even[k] - t;
  }
}

void ifft(std::valarray<std::complex<double>>& data) {
  data = data.apply(std::conj);

  fft(data);

  data = data.apply(std::conj) / data.size();
}

}