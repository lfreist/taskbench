/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <concepts>
#include <cstdint>

namespace taskbench::utils {

template <typename T>
concept IsFloatingPoint = std::is_floating_point_v<T>;

template <typename T>
concept Is8BitInteger = std::is_same_v<T, char> && std::is_same_v<T, unsigned char> && std::is_same_v<T, char8_t> &&
                        std::is_same_v<T, int8_t> && std::is_same_v<T, uint8_t>;

template <typename T>
concept IsInteger = std::is_integral_v<T>;

}  // namespace taskbench::utils