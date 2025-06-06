# MeMS (Memory Management System)

MeMS (Memory Management System) is a memory allocation system designed to efficiently manage memory segments, including both process segments and hole segments. This README provides an overview of how the MeMS system works and how to use it.

## Table of Contents
1. [Overview](#overview)
2. [Initialization](#initialization)
3. [Memory Allocation](#memory-allocation)
4. [Memory Deallocation](#memory-deallocation)
5. [Printing System Statistics](#printing-system-statistics)
6. [Cleaning Up](#cleaning-up)
7. [Getting Physical Address](#getting-physical-address)
8. [Dependencies](#dependencies)
9. [Usage](#usage)
10. [License](#license)

## Overview

MeMS is built around the concept of dividing memory into main chains, each of which contains a linked list of sub-chains. Sub-chains can represent either hole segments (unused memory) or process segments (allocated memory). The system is designed to efficiently manage memory allocation, minimize fragmentation, and optimize memory usage.

### Data Structures

The MeMS system uses two main data structures to organize memory: `MainChainNode` and `SubChainNode`.

#### MainChainNode

A `MainChainNode` represents a main chain. It has the following attributes:

- `start`: The starting address of the main chain.
- `size`: The size of the main chain.
- `sub_chain`: A linked list of `SubChainNode` instances representing memory segments within the main chain.
- `next`: A pointer to the next main chain node.
- `prev`: A pointer to the previous main chain node.

#### SubChainNode

A `SubChainNode` represents a sub-chain, which can be either a hole (unused memory) or a process (allocated memory). It has the following attributes:

- `start`: The starting address of the memory segment.
- `size`: The size of the memory segment.
- `type`: Indicates whether the segment is a hole (`0`) or a process (`1`).
- `next`: A pointer to the next sub-chain node.
- `prev`: A pointer to the previous sub-chain node.

## Initialization

### `mems_init()`

The MeMS system is initialized using the `mems_init()` function:
- A single main chain node is created with the full available memory (typically one page of memory) using the `mmap` system call.
- The main chain node is added to the free list, and the `mems_virtual_address` and `mems_physical_address` are set to the starting address of this main chain.

## Memory Allocation

### `mems_malloc(size_t size)`

The `mems_malloc()` function is used to allocate memory. It follows these steps:
- It iterates through the main chains and their sub-chains to find a suitable hole segment for allocation. The goal is to find a hole segment with a size greater than or equal to the requested size.
- If a suitable hole segment is found:
  - The function calculates the total size of adjacent hole segments (if any) and removes them from the linked list of sub-chains.
  - If the total hole size is larger than or equal to the requested size, it splits the hole, allocating a new process segment, and creating a new hole segment with the remaining memory.
  - If the total hole size is just enough for the requested size, it allocates the entire hole as a process segment.
- If no suitable hole segment is found, a new main chain node is created using the `mmap` system call. This new node represents a main chain that contains a single process segment. A new sub-chain node is allocated within this main chain to represent the process segment.
- The function returns the starting address of the allocated process segment.

## Memory Deallocation

### `mems_free(void* v_ptr)`

The `mems_free()` function is used to deallocate memory:
- It takes a MeMS virtual address (`v_ptr`) and marks the corresponding memory segment as a hole (type 0).
- The function then merges adjacent hole segments in both the sub-chain and main chain to reduce memory fragmentation. If there are adjacent hole segments, they are combined into a single larger hole.

## Printing System Statistics

### `mems_print_stats()`

The `mems_print_stats()` function is used to display system statistics:
- It prints information about the main chains, sub-chains, the number of pages used, space unused, main chain length, and sub-chain lengths.
- This information is valuable for monitoring the memory allocation and utilization of the MeMS system.

## Cleaning Up

### `mems_finish()`

The `mems_finish()` function is used to release the allocated memory and clean up the MeMS system. It unmaps all memory segments and frees system resources, ensuring that there are no memory leaks.

## Getting Physical Address

### `mems_get(void* v_ptr)`

The `mems_get()` function is provided to convert a MeMS virtual address (`v_ptr`) into a physical address. However, it currently only returns the same virtual address.

## Dependencies

- Standard C libraries
- Memory mapping functions (e.g., `mmap`, `munmap`)
- No external dependencies required

## Usage

1. **Initialize the MeMS system:**
    ```c
    mems_init();
    ```

2. **Allocate memory:**
    ```c
    void* ptr = mems_malloc(size);
    ```

3. **Free memory:**
    ```c
    mems_free(ptr);
    ```

4. **Print statistics:**
    ```c
    mems_print_stats();
    ```

5. **Cleanup:**
    ```c
    mems_finish();
    ```

## License

This project is provided for educational purposes. See the project repository for license information.
