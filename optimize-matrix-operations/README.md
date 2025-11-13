# Project: Matrix Operations Performance Analysis

This document details the performance analysis of different matrix multiplication and transposition algorithms, including the results from the experiments. The C code for these experiments is provided in the accompanying files `matmul-naive.c` and `transpose-cache-blocked.c`. The analysis focuses on the impact of memory access patterns and cache blocking on performance, measured in GFLOPS (giga-floating-point operations per second).

## Matrix Multiplication

The `matmul-naive.c` file contains a straightforward `ijk` implementation of matrix multiplication. The order of the loops significantly impacts performance because of how data is stored and accessed in memory.

### Key Findings

*   **Loop Order**: The `ikj` loop ordering typically performs best because it accesses memory in a contiguous manner, maximizing spatial locality and reducing cache misses. Changing the loop order can lead to substantial performance gains, sometimes as much as 5x to 20x.
*   **Inefficient Loop Order**: The `jik` and `ijk` loop orderings are generally less performant. With row-major memory storage, these orderings can lead to a high number of cache misses because they jump through memory instead of accessing adjacent elements.

### Results

<img width="353" height="98" alt="image" src="https://github.com/user-attachments/assets/308572e7-2bc1-405e-b567-297cbbda149e" />


## Matrix Transposition

The `transpose-cache-blocked.c` file contains a naive implementation and a more optimized cache-blocked (tiled) implementation for matrix transposition. The experiments were run with two different settings to analyze the effect of block size on performance.

### Setting 1

In this setting, the matrix size was kept constant while the block size was varied to observe its effect on performance.

*   **Matrix Size (N):** 1000
*   **Block Sizes:** 100, 1000, 2000, 5000, 10000

#### Results

The cache-blocked version became faster than the naive version at a block size of 1000. With small blocks, the overhead of managing them is high, while very large blocks can lead to cache thrashing.

| Block Size | Naive GFLOPS | Blocked GFLOPS |
| :--- | :--- | :--- |
| 100 | 46.51 | 43.48 |
| 1000 | 514.67 | 543.48 |
| 2000 | 675.05 | 1223.43 |
| 5000 | 1057.59 | 2867.27 |
| 10000 | 981.25 | 5382.84 |

*Table data sourced from below info*

<img width="307" height="189" alt="image" src="https://github.com/user-attachments/assets/ce68632a-a354-495d-8a2a-4922a00b4478" />


### Setting 2

In this setting, a larger matrix was used, and the performance was tested against several block sizes.

*   **Matrix Size (N):** 10000
*   **Block Sizes:** 50, 100, 500, 1000, 5000

#### Results

As block size increases, performance first improves due to a higher cache hit rate. However, the benefit diminishes and performance becomes neutral as the block size approaches the limits of the cache.

| Block Size | Naive GFLOPS | Blocked GFLOPS |
| :--- | :--- | :--- |
| 50 | 383.51 | 499.50 |
| 100 | 376.65 | 680.97 |
| 500 | 250.50 | 438.89 |
| 1000 | 329.71 | 629.13 |
| 5000 | 339.27 | 550.51 |

*Table data sourced from below observation*

<img width="308" height="187" alt="image" src="https://github.com/user-attachments/assets/c8e4aa91-2694-4387-896c-3d4c950df1cd" />


## How to Run

1.  **Compile the code:**
    ```
    gcc -o matmul matmul-naive.c
    gcc -o transpose transpose-cache-blocked.c
    ```

2.  **Execute the compiled programs:**
    ```
    ./matmul
    ./transpose
    ```

The output will display the performance in GFLOPS. You can modify the `#define` values in the C files to replicate the experiments.
