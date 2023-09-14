# taskbench
A C++ Library implementing different common tasks for benchmarking purposes

## Build & Run

### Linux
```bash
mkdir build && cd build
cmake .. && make -j
./lib/taskbench
```

## Example Output

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