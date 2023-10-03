/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/concepts.h>

#include <Eigen/Dense>
#include <concepts>
#include <cstddef>

#ifdef _MSC_VER
using ssize_t = __int64;
#endif

namespace taskbench::cpu::mmul {

Eigen::MatrixXd build_matrix(ssize_t m, ssize_t n, int seed);

Eigen::MatrixXd matrix_multiplication(const Eigen::MatrixXd& a, const Eigen::MatrixXd& b);

}  // namespace taskbench::cpu::mmul