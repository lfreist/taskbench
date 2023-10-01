/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/cpu/mmul.h>

#include <random>

namespace taskbench::cpu::mmul {

Eigen::MatrixXd build_matrix(ssize_t m, ssize_t n, int seed) {
  std::mt19937 rng;
  rng.seed(seed);

  return Eigen::MatrixXd::Random(m, n);
}

Eigen::MatrixXd matrix_multiplication(const Eigen::MatrixXd& a, const Eigen::MatrixXd& b) { return a * b; }

}  // namespace taskbench::cpu::mmul