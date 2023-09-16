![c++20](https://github.com/lfreist/taskbench/.images/badges/c++20.svg)
![CMake39](https://github.com/lfreist/taskbench/.images/badges/cmake39.svg)

[![Linux (clang)](https://github.com/lfreist/taskbench/actions/workflows/build-linux-clang-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-linux-clang-cmake.yml)
[![Linux (gcc)](https://github.com/lfreist/taskbench/actions/workflows/build-linux-gcc-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-linux-gcc-cmake.yml)

[![MacOS](https://github.com/lfreist/taskbench/actions/workflows/build-macos-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-macos-cmake.yml)

[![Windows (Visual Studio)](https://github.com/lfreist/taskbench/actions/workflows/build-win-msvc-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-win-msvc-cmake.yml)
[![Windows (Cygwin)](https://github.com/lfreist/taskbench/actions/workflows/build-win-cygwin-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-win-cygwin-cmake.yml)
[![Windows (MinGW)](https://github.com/lfreist/taskbench/actions/workflows/build-win-mingw-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-win-mingw-cmake.yml)
[![Windows (MSYS2)](https://github.com/lfreist/taskbench/actions/workflows/build-win-msys2-cmake.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-win-msys2-cmake.yml)

[![Thread Sanitizer](https://github.com/lfreist/taskbench/actions/workflows/build-thread-sanitizer.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-thread-sanitizer.yml)
[![Address Sanitizer](https://github.com/lfreist/taskbench/actions/workflows/build-address-sanitizer.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/build-address-sanitizer.yml)

[![Code Style](https://github.com/lfreist/taskbench/actions/workflows/format-check.yml/badge.svg)](https://github.com/lfreist/taskbench/actions/workflows/format-check.yml)

# taskbench

A C++ Library implementing different common tasks for benchmarking purposes

## Build

Taskbenchs desired build system is CMake. This has several reasons but the most important is the convenience that all
used external libraries build using CMake. Thus, all dependencies are included into this repository and build together
with taskbench.

### Requirements

- A C++ compiler supporting C++20 features (including concepts):
  - g++>=10
  - clang>=10
  - msvc>=16.3
- CMake (Version 3.9)
- OpenCL (Your GPU drivers already provide OpenCL. Only install OpenCL if you have no external GPU.)
  - Windows: [OpenCL™ and OpenGL® Compatibility Pack](https://apps.microsoft.com/store/detail/opencl%E2%84%A2-and-opengl%C2%AE-compatibility-pack/9NQPSL29BFFF?hl=en-en&gl=en)
  - Linux: `ocl-icd-opencl-dev` - Search for your specific distribution and package manage...

### Dependencies

`taskbench` depends on the following libraries:

- [ZStandard](https://github.com/facebook/zstd)
- [lfreist/AES](https://github.com/lfreist/AES) (fork of [SergeyBel/AES](https://github.com/SergeyBel/AES))
- [fmtlib](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)
- [miss-ocl](https://github.com/lfreist/miss-ocl) (An OpenCL wrapper including the OpenCL headers)

All these dependencies are included into this repository. No need to install anything!

### Build

```bash
git submodule update --init --recursive
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

This builds all target libraries as *shared* and *static* libraries.

## Include taskbench to your CMake project

1. Add `taskbench` to your project. Choose one of these options:
   1. Clone it from GitHub into `external/taskbench`: `git clone https://github.com/lfreist/taskbench.git`
   2. Include it as submodule (preferred): `git submodule add https://github.com/lfreist/taskbench.git external/taskbench`
2. Add the following to your projects root CMakeLists.txt:
   ```cmake
   # Your setup ...
   include_directories(external/taskbench/include)
   add_subdirectory(external/taskbench)
   # ...
   ```
3. Include `taskbench` to your source files:
   ```c++
   // main.cpp
   
   #include <taskbench/taskbench.h>
   
   int main(int argc, char** argv) {
     // run all CPU benchmarks
     taskbench::cpu::Benchmark cpu_benchmark;
     cpu_benchmark.run_all(taskbench::seconds(3));
     return 0;
   }
   ```
4. Link against `taskbench` or `taskbench_static`:
   ```cmake
   # ...
   add_executable(main main.cpp)
   target_link_libraries(main taskbench::taskbench_static)
   ```

## Run BenchBlitz

Taskbench provides a command line tool `BenchBlitz` that runs the implemented benchmark tasks and prints out the
results. `BenchBlitz` is built together with the library as described above.

```bash
./build/taskbench/BenchBlitz # Linux/MacOS
.\build\taskbench\BenchBlitz # Windows
```

### Example Output

```
RAM Benchmarks:
    read                 (0.018 +/- 0.001) s [59.91 GiB/s]
    write                (0.017 +/- 0.001) s [66.17 GiB/s]
    read/write           (0.020 +/- 0.003) s [67.54 GiB/s]
CPU Benchmarks:
  AES Benchmarks:
    AES encryption       (1.330 +/- 0.005) s 
    AES decryption       (1.417 +/- 0.001) s 
  ZStandard Benchmarks:
    ZSTD compression     (0.874 +/- 0.001) s 
    ZSTD decompression   (0.769 +/- 0.000) s 
  FFT Benchmarks:
    FFT                  (0.722 +/- 0.006) s 
    Inverse FFT          (0.762 +/- 0.003) s 
  Matrix Multiplication Benchmarks:
    mmul (int)           (1.407 +/- 0.001) s [1.53 G op/s]
    mmul (double)        (1.653 +/- 0.004) s [1.30 G op/s]
  Sort Benchmarks:
    sort (int)           (2.057 +/- 0.001) s 
    sort (double)        (2.371 +/- 0.001) s 
    sort (string)        (1.188 +/- 0.000) s 
  Synthetic Benchmarks:
    add/sub (int)        (1.264 +/- 0.009) s [3.18 G op/s]
    mul (int)            (1.901 +/- 0.000) s [2.10 G op/s]
    div (int)            (1.096 +/- 0.000) s [365.23 M op/s]
    add/sub (double)     (1.567 +/- 0.000) s [2.55 G op/s]
    mul (double)         (3.216 +/- 0.000) s [1.24 G op/s]
    div (int)            (0.901 +/- 0.169) s [530.09 M op/s]
GPU Benchmarks:
  Matrix Multiplication Benchmarks:
    mmul                 (1.471 +/- 0.009) s [11.75 G op/s]
  Synthetic Benchmarks:
    integer ops          (0.982 +/- 0.076) s [596.29 G op/s]
    float ops            (0.328 +/- 0.002) s [1.69 T op/s]
  Memory Benchmarks:
    memory write         (0.020 +/- 0.000) s [51.35 GiB/s]
    memory read          (0.016 +/- 0.000) s [61.86 GiB/s]
  Transfer Benchmarks:
    write to device      (0.074 +/- 0.000) s [13.51 GiB/s]
    read from device     (0.074 +/- 0.000) s [13.50 GiB/s]
```