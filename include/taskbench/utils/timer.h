/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <chrono>

namespace taskbench::utils {

/**
 * @brief Simple (stop watch like) timer class using std::chrono::high_resolution_clock
 */
class Timer {
  typedef std::chrono::high_resolution_clock clock;

 public:
  Timer() = default;

  /**
   * @brief Start the timer
   */
  void start() noexcept;

  /**
   * @brief Stop the timer and return the time difference between start() and stop() call
   * @return
   */
  std::chrono::duration<double> stop() noexcept;

  /**
   * @brief Get the current time since start without stopping the timer
   * @return
   */
  std::chrono::duration<double> count() noexcept;

  /**
   * @brief Returns count and restarts the timer
   * @return
   */
  std::chrono::duration<double> round() noexcept;

 private:
  std::chrono::time_point<clock, std::chrono::nanoseconds> _start;
};

}  // namespace taskbench::utils