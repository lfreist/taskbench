/**
 * Copyright 2023, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 *
 * This file is part of taskbench.
 */

#include <taskbench/tasks/gpu/synthetic.h>

namespace taskbench::gpu::synthetic {

// _____________________________________________________________________________________________________________________
void mem_write(mcl::Kernel& kernel) {
  kernel.run();
}

// _____________________________________________________________________________________________________________________
void mem_read(mcl::Kernel& kernel) {
  kernel.run();
}

// _____________________________________________________________________________________________________________________
void ops_fp(mcl::Kernel& kernel) {
  kernel.run();
}

// _____________________________________________________________________________________________________________________
void ops_int(mcl::Kernel& kernel) {
  kernel.run();
}

}