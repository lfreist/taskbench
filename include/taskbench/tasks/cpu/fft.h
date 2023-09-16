/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <complex>
#include <valarray>

#define M_PI 3.14159265358979323846

namespace taskbench::cpu::fft {

/**
 * @brief Fast Fourier Transformation (FFT)
 * @param src
 * @param dst
 */
void fft(std::valarray<std::complex<double>>& data);

/**
 * @brief Inverse Fast Fourier Transformation
 * @param src
 * @param dst
 */
void ifft(std::valarray<std::complex<double>>& data);

}  // namespace taskbench::cpu::fft