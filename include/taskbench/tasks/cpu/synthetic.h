/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <concepts>
#include <cstddef>
#include <numeric>

namespace taskbench::cpu::synthetic {

template <typename T>
  requires utils::IsInteger<T> || utils::IsFloatingPoint<T>
T add_sub(size_t num_operations_div_100, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9) {
  volatile T r0 = static_cast<T>(0);
  volatile T r1 = static_cast<T>(0);
  volatile T r2 = static_cast<T>(0);
  volatile T r3 = static_cast<T>(0);
  volatile T r4 = static_cast<T>(0);
  volatile T r5 = static_cast<T>(0);
  volatile T r6 = static_cast<T>(0);
  volatile T r7 = static_cast<T>(0);
  volatile T r8 = static_cast<T>(0);
  volatile T r9 = static_cast<T>(0);
  for (size_t i = 0; i < num_operations_div_100; ++i) {
    r0 += v0;
    r1 -= v1;
    r2 += v2;
    r3 -= v3;
    r4 += v4;
    r5 -= v5;
    r6 += v6;
    r7 -= v7;
    r8 += v8;
    r9 -= v9;

    r1 += v0;
    r2 -= v1;
    r3 += v2;
    r4 -= v3;
    r5 += v4;
    r6 -= v5;
    r7 += v6;
    r8 -= v7;
    r9 += v8;
    r0 -= v9;

    r2 += v0;
    r3 -= v1;
    r4 += v2;
    r5 -= v3;
    r6 += v4;
    r7 -= v5;
    r8 += v6;
    r9 -= v7;
    r0 += v8;
    r1 -= v9;

    r3 += v0;
    r4 -= v1;
    r5 += v2;
    r6 -= v3;
    r7 += v4;
    r8 -= v5;
    r9 += v6;
    r0 -= v7;
    r1 += v8;
    r2 -= v9;

    r4 += v0;
    r5 -= v1;
    r6 += v2;
    r7 -= v3;
    r8 += v4;
    r9 -= v5;
    r0 += v6;
    r1 -= v7;
    r2 += v8;
    r3 -= v9;

    r5 += v0;
    r6 -= v1;
    r7 += v2;
    r8 -= v3;
    r9 += v4;
    r0 -= v5;
    r1 += v6;
    r2 -= v7;
    r3 += v8;
    r4 -= v9;

    r6 += v0;
    r7 -= v1;
    r8 += v2;
    r9 -= v3;
    r0 += v4;
    r1 -= v5;
    r2 += v6;
    r3 -= v7;
    r4 += v8;
    r5 -= v9;

    r7 += v0;
    r8 -= v1;
    r9 += v2;
    r0 -= v3;
    r1 += v4;
    r2 -= v5;
    r3 += v6;
    r4 -= v7;
    r5 += v8;
    r6 -= v9;

    r8 += v0;
    r9 -= v1;
    r0 += v2;
    r1 -= v3;
    r2 += v4;
    r3 -= v5;
    r4 += v6;
    r5 -= v7;
    r6 += v8;
    r7 -= v9;

    r9 += v0;
    r0 -= v1;
    r1 += v2;
    r2 -= v3;
    r3 += v4;
    r4 -= v5;
    r5 += v6;
    r6 -= v7;
    r7 += v8;
    r8 -= v9;
  }
  return r0 + r1 + r2 + r3 + r4 - r5 - r6 - r7 - r8 - r9;
}

template <typename T>
  requires utils::IsInteger<T> || utils::IsFloatingPoint<T>
void mul(size_t num_operations_div_100, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T threshold) {
  volatile T r0 = static_cast<T>(1);
  volatile T r1 = static_cast<T>(1);
  volatile T r2 = static_cast<T>(1);
  volatile T r3 = static_cast<T>(1);
  volatile T r4 = static_cast<T>(1);

  for (size_t i = 0; i < num_operations_div_100; ++i) {
    if (r0 > threshold) {
      r0 = 1;
    }
    if (r1 > threshold) {
      r1 = 1;
    }
    if (r2 > threshold) {
      r2 = 1;
    }
    if (r3 > threshold) {
      r3 = 1;
    }
    if (r4 > threshold) {
      r4 = 1;
    }
    r0 *= v0;
    r1 *= v1;
    r2 *= v2;
    r3 *= v3;
    r4 *= v4;
    r0 *= v5;
    r1 *= v6;
    r2 *= v7;
    r3 *= v8;
    r4 *= v9;

    r1 *= v0;
    r2 *= v1;
    r3 *= v2;
    r4 *= v3;
    r0 *= v4;
    r1 *= v5;
    r2 *= v6;
    r3 *= v7;
    r4 *= v8;
    r0 *= v9;

    r2 *= v0;
    r3 *= v1;
    r4 *= v2;
    r0 *= v3;
    r1 *= v4;
    r2 *= v5;
    r3 *= v6;
    r4 *= v7;
    r0 *= v8;
    r1 *= v9;

    r3 *= v0;
    r4 *= v1;
    r0 *= v2;
    r1 *= v3;
    r2 *= v4;
    r3 *= v5;
    r4 *= v6;
    r0 *= v7;
    r1 *= v8;
    r2 *= v9;

    r4 *= v0;
    r0 *= v1;
    r1 *= v2;
    r2 *= v3;
    r3 *= v4;
    r4 *= v5;
    r0 *= v6;
    r1 *= v7;
    r2 *= v8;
    r3 *= v9;

    r0 *= v0;
    r1 *= v1;
    r2 *= v2;
    r3 *= v3;
    r4 *= v4;
    r0 *= v5;
    r1 *= v6;
    r2 *= v7;
    r3 *= v8;
    r4 *= v9;

    r0 *= v0;
    r1 *= v1;
    r2 *= v2;
    r3 *= v3;
    r0 *= v4;
    r1 *= v5;
    r2 *= v6;
    r3 *= v7;
    r4 *= v8;
    r0 *= v9;

    r2 *= v0;
    r3 *= v1;
    r4 *= v2;
    r0 *= v3;
    r1 *= v4;
    r2 *= v5;
    r3 *= v6;
    r4 *= v7;
    r0 *= v8;
    r1 *= v9;

    r3 *= v0;
    r4 *= v1;
    r0 *= v2;
    r1 *= v3;
    r2 *= v4;
    r3 *= v5;
    r4 *= v6;
    r0 *= v7;
    r1 *= v8;
    r2 *= v9;

    r4 *= v0;
    r0 *= v1;
    r1 *= v2;
    r2 *= v3;
    r3 *= v4;
    r4 *= v5;
    r0 *= v6;
    r1 *= v7;
    r2 *= v8;
    r3 *= v9;
  }
}


template <typename T>
  requires utils::IsInteger<T> || utils::IsFloatingPoint<T>
void div(size_t num_operations_div_100, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9) {
  T max = std::numeric_limits<T>::max();
  volatile T r0 = max;
  volatile T r1 = max;
  volatile T r2 = max;
  volatile T r3 = max;
  volatile T r4 = max;

  for (size_t i = 0; i < num_operations_div_100; ++i) {
    if (r0 < 0) {
      r0 = max;
    }
    if (r1 < 0) {
      r1 = max;
    }
    if (r2 < 0) {
      r2 = max;
    }
    if (r3 < 0) {
      r3 = max;
    }
    if (r4 < 0) {
      r4 = max;
    }
    r0 /= v0;
    r1 /= v1;
    r2 /= v2;
    r3 /= v3;
    r4 /= v4;
    r0 /= v5;
    r1 /= v6;
    r2 /= v7;
    r3 /= v8;
    r4 /= v9;

    r1 /= v0;
    r2 /= v1;
    r3 /= v2;
    r4 /= v3;
    r0 /= v4;
    r1 /= v5;
    r2 /= v6;
    r3 /= v7;
    r4 /= v8;
    r0 /= v9;

    r2 /= v0;
    r3 /= v1;
    r4 /= v2;
    r0 /= v3;
    r1 /= v4;
    r2 /= v5;
    r3 /= v6;
    r4 /= v7;
    r0 /= v8;
    r1 /= v9;

    r3 /= v0;
    r4 /= v1;
    r0 /= v2;
    r1 /= v3;
    r2 /= v4;
    r3 /= v5;
    r4 /= v6;
    r0 /= v7;
    r1 /= v8;
    r2 /= v9;

    r4 /= v0;
    r0 /= v1;
    r1 /= v2;
    r2 /= v3;
    r3 /= v4;
    r4 /= v5;
    r0 /= v6;
    r1 /= v7;
    r2 /= v8;
    r3 /= v9;

    r0 /= v0;
    r1 /= v1;
    r2 /= v2;
    r3 /= v3;
    r4 /= v4;
    r0 /= v5;
    r1 /= v6;
    r2 /= v7;
    r3 /= v8;
    r4 /= v9;

    r0 /= v0;
    r1 /= v1;
    r2 /= v2;
    r3 /= v3;
    r0 /= v4;
    r1 /= v5;
    r2 /= v6;
    r3 /= v7;
    r4 /= v8;
    r0 /= v9;

    r2 /= v0;
    r3 /= v1;
    r4 /= v2;
    r0 /= v3;
    r1 /= v4;
    r2 /= v5;
    r3 /= v6;
    r4 /= v7;
    r0 /= v8;
    r1 /= v9;

    r3 /= v0;
    r4 /= v1;
    r0 /= v2;
    r1 /= v3;
    r2 /= v4;
    r3 /= v5;
    r4 /= v6;
    r0 /= v7;
    r1 /= v8;
    r2 /= v9;

    r4 /= v0;
    r0 /= v1;
    r1 /= v2;
    r2 /= v3;
    r3 /= v4;
    r4 /= v5;
    r0 /= v6;
    r1 /= v7;
    r2 /= v8;
    r3 /= v9;
  }
}

}  // namespace taskbench::cpu::synthetic