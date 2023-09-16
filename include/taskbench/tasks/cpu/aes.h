/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#pragma once

#include <vector>

namespace taskbench::cpu::aes {

/**
 * @brief Encrypt data using AES encryption
 * @param data
 * @param key
 * @return
 */
std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data, const std::vector<unsigned char>& key);

/**
 * @brief Decrypt data using AES decryption
 * @param data
 * @param key
 * @return
 */
std::vector<unsigned char> decrypt(const std::vector<unsigned char>& data, const std::vector<unsigned char>& key);

}  // namespace taskbench::cpu::aes