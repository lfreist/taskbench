/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/utils/timer.h>

namespace taskbench::utils {

// _____________________________________________________________________________________________________________________
void Timer::start() noexcept {
  _start = clock::now();
}

// _____________________________________________________________________________________________________________________
std::chrono::duration<double> Timer::stop() noexcept {
  auto end = clock::now();
  return end - _start;
}

}