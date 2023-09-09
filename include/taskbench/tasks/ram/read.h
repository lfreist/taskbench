/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <taskbench/utils/data_generator.h>

#include <cstddef>
#include <vector>

namespace taskbench::ram::read {

/**
 * @brief Iterate through data and check if values are '\0'.
 * !! data must not contain '\0' !!
 *
 * !! Function should be called on multiple threads to ensure the operations are not CPU bound !!
 *
 * @param data const char* not containing '\0'
 * @param size size of data
 */
void sequential(const char* data, size_t size);

/**
 * @brief Iterate through values in positions that are used to access corresponding data entry.
 * !! Values in positions must be at most size of data !!
*
* !! Function should be called on multiple threads to ensure the operations are not CPU bound !!
*
 * @param data
 * @param positions
 */
void random(utils::Node<char>* start, size_t size);

}  // namespace taskbench::ram::read