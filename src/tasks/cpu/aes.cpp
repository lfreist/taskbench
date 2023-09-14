/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <AES.h>
#include <taskbench/tasks/cpu/aes.h>

namespace taskbench::cpu::aes {

// _____________________________________________________________________________________________________________________
std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data, const std::vector<unsigned char>& key) {
  AES aes(AESKeyLength::AES_256);
  return aes.EncryptECB(data, key);
}

// _____________________________________________________________________________________________________________________
std::vector<unsigned char> decrypt(const std::vector<unsigned char>& data, const std::vector<unsigned char>& key) {
  AES aes(AESKeyLength::AES_256);
  return aes.DecryptECB(data, key);
}

}  // namespace taskbench::cpu::aes