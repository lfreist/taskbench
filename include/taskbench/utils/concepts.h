/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <concepts>

namespace taskbench::utils {

template <typename T>
concept IsFloatingPoint = std::is_floating_point_v<T>;

template <typename T>
concept IsInteger = std::is_integral_v<T>;

}