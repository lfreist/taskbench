/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <fmt/core.h>

namespace taskbench::utils {

/**
 * @brief Format a op/s value to Kilo (K), Mega (M), Giga (G) or Terra (T) op/s
 * @tparam T integer or floating point type
 * @param val op/s value
 * @return
 */
template <typename T>
  requires IsInteger<T> || IsFloatingPoint<T>
std::string pretty_ops(T val) {
  if (val < 1000) {
    return fmt::format("{:.2f} op/s", val);
  }
  if (val < 1000000) {
    return fmt::format("{:.2f} K op/s", static_cast<double>(val) / 1000.0f);
  }
  if (val < 1000000000) {
    return fmt::format("{:.2f} M op/s", static_cast<double>(val) / 1000000.0f);
  }
  if (val < 1000000000000) {
    return fmt::format("{:.2f} G op/s", static_cast<double>(val) / 1000000000.0f);
  }
  return fmt::format("{:.2f} T op/s", static_cast<double>(val) / 1000000000000.0f);
}

}