# RISC-V Cache Performance Analysis using Venus Simulator

## Overview

This project analyzes the cache performance of a C program translated into RISC-V assembly language `risc-v-cache-venus.S`. The Venus web-based simulator is used to execute the RISC-V code and measure cache metrics like hit rates and memory access patterns under different configurations. The analysis is conducted under three distinct settings, each with a unique combination of program parameters and cache architectures, to explore their impact on performance.

## Project Structure

*   `risc-v-cache-venus.S`: The RISC-V assembly code translated from the below stated C source. This file should be loaded into the Venus simulator for execution.
*   `README.md`: This file, providing a complete overview of the project, simulation instructions, and a summary of the results.

### Original C Code

The following C code serves as the basis for the RISC-V program and the subsequent cache simulation analysis:

```
#define size 1024
int main() {
    int reps, int step, option, i;
    reps=10;
    step=2;
    option=1;
    char a[size];
    for(i=0;reps;i++) {
        for(j=0;j<size; j+=step){
            if(option==0)
                a=0;
            else
                a[j]+=1;
        }
    }
    return 1;
}
```

## How to Simulate

To replicate the results, follow these steps:

1.  Navigate to the [Venus RISC-V Simulator](https://venus.cs61c.org/).
2.  Load the RISC-V assembly code (`risc-v-cache-venus.S`) into the "Editor" tab.
3.  In the assembly code, initialize the appropriate `a` registers to set the Program Parameters (`Array Size`, `Step Size`, `Rep Count`, `Option`) for each specific setting.
4.  Navigate to the "Cache" tab to configure the cache parameters according to the desired setting.
5.  Run the simulation and observe the cache statistics in the "Cache" tab to analyze performance.

## Simulation Settings & Results

### Setting 1: Basic Direct-Mapped Cache
This setting uses a small, simple, direct-mapped cache to introduce the basic concepts of cache hits and misses.

| Parameter | Value |
| :--- | :--- |
| **Program Parameters** | |
| `Array Size` | 128 bytes |
| `Step Size` | 8 |
| `Rep Count` | 4 |
| `Option` | 0 |
| **Cache Parameters** | |
| `Cache Levels` | 1 |
| `Block Size` | 8 bytes |
| `Number of Blocks` | 4 |
| `Placement Policy` | Direct Mapped |
| `Associativity` | 1 |
| `Block Replacement`| LRU |

#### Results
*   **Observed Hit Rate:** 0.0
*   **Reasoning:** The combination of the step size and the total cache size results in conflict misses. The cache is too small to store all the data being accessed in the loop, leading to constant evictions and a complete lack of cache hits. With a step size of 8 and only 4 cache blocks of 8 bytes each, the memory access pattern causes different data elements to map to the same cache block, resulting in repeated evictions and re-fetches.
*   **Improvement:** The hit rate could be improved by increasing the `Block size` to 16 bytes, which would allow more consecutive data to be fetched into the cache at once, potentially capturing more of the data accessed in subsequent loop iterations.

<p align="center">
<img width="298" height="326" alt="Setting 1 Cache Statistics" src="https://github.com/user-attachments/assets/9f084342-a182-4d3a-a1b5-f9258dea7389" />
</p>
  
### Setting 2: Set-Associative Cache
This setting introduces a more complex cache with a higher associativity to explore more nuanced cache behaviors and hit/miss patterns.

| Parameter | Value |
| :--- | :--- |
| **Program Parameters** | |
| `Array Size` | 256 bytes |
| `Step Size` | 2 |
| `Rep Count` | 1 |
| `Option` | 1 |
| **Cache Parameters** | |
| `Cache Levels` | 1 |
| `Block Size` | 16 bytes |
| `Number of Blocks` | 16 |
| `Placement Policy` | N-Way Set Associative |
| `Associativity` | 4 |
| `Block Replacement`| LRU |

#### Results
*   **Repeating Hit/Miss Pattern:** MHH (Miss, Hit, Hit)
*   **Hit Rate with High Rep Count:** As the `Rep Count` approaches infinity, the hit rate converges to **0.75** (or 75%). This is because after the initial compulsory misses, the data remains in the cache for subsequent accesses within the loops. The pattern consists of one miss followed by two hits, yielding a 2/3 ≈ 0.67 hit rate per three-access sequence, which improves with additional repetitions.
*   **Analysis:** The 4-way set-associative cache with 16 blocks provides sufficient capacity and flexibility to retain frequently accessed data. The larger block size (16 bytes) also captures more contiguous data elements, improving spatial locality.

<p align="center">
<img width="319" height="335" alt="Setting 2 Cache Statistics" src="https://github.com/user-attachments/assets/e7316f5f-473b-4167-bf6a-adf969fff1e9" />
</p>

### Setting 3: Two-Level Cache Hierarchy
This setting introduces a multi-level cache hierarchy with both L1 and L2 caches, allowing for an analysis of how these levels interact.

| Parameter | Value |
| :--- | :--- |
| **Program Parameters** | |
| `Array Size` | 128 bytes |
| `Step Size` | 1 |
| `Rep Count` | 1 |
| `Option` | 0 |
| **L1 Cache** | |
| `Block Size` | 8 bytes |
| `Number of Blocks` | 8 |
| `Placement Policy` | Direct Mapped |
| **L2 Cache** | |
| `Block Size` | 8 bytes |
| `Number of Blocks` | 16 |
| `Placement Policy` | Direct Mapped |

#### Results
*   **Cache Hit Rates:**
    *   **L1 Hit Rate:** 0.5 (50%)
    *   **L2 Hit Rate:** 0.0 (0%)
    *   **Overall Hit Rate:** 0.5 (50%)
*   **Cache Accesses:**
    *   **L1 Accesses/Misses:** There are a total of 259 L1 accesses, with 129 of them being misses.
    *   **L2 Accesses:** The 129 misses in the L1 cache result in 128 accesses to the L2 cache.
*   **Performance Observations:** The L2 cache is ineffective in this scenario, contributing no hits and therefore providing no performance benefit. The misses that occur in the L1 cache are also misses in the L2 cache, indicating that the L2 cache does not contain the required data either. The L2 hit rate could be improved by increasing its associativity from direct-mapped to 2-way or 4-way set-associative, which would help mitigate conflict misses that are not resolved by the L1 cache.

<p align="center">
<img width="193" height="319" alt="Setting 3 Cache Statistics" src="https://github.com/user-attachments/assets/998d2ad2-27f3-4dae-8f7f-b4f8ce81620d" />
</p>

## Key Findings

1. **Direct-Mapped Limitations:** Setting 1 demonstrates how small direct-mapped caches can suffer from severe conflict misses, resulting in 0% hit rates when the access pattern and cache size are poorly matched.

2. **Set-Associativity Benefits:** Setting 2 shows that increasing associativity significantly improves cache performance, achieving a 75% hit rate with sufficient repetitions due to reduced conflict misses.

3. **Cache Hierarchy Challenges:** Setting 3 reveals that simply adding a second cache level does not guarantee improved performance. Both cache levels must be properly sized and configured to effectively capture different working set characteristics.

## Conclusion

This project demonstrates the critical importance of cache design parameters in determining system performance. The choice of cache size, associativity, block size, and hierarchy configuration must be carefully matched to the expected memory access patterns to achieve optimal performance. The Venus simulator provides an accessible platform for exploring these concepts and understanding their practical implications for RISC-V processor design.

## References

- [Venus RISC-V Simulator](https://venus.cs61c.org/)
- RISC-V Instruction Set Manual
- Computer Architecture: A Quantitative Approach (Hennessy & Patterson)
