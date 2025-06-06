---

# Parent-Child Relationship: Process Creation and Synchronization in C

This directory contains C programs demonstrating process creation, parent-child relationships, and process synchronization using `fork()`, `wait()`, and inter-process communication concepts. The tasks require the parent and child to calculate Fibonacci numbers and factorials, respectively, and to coordinate their outputs as specified.

---

## Problem Statements

### Part - I

- **Create a parent and child process using `fork()`.**
- **Parent prints:**
  - `Parent (P) is having ID <PID>`
  - `ID of P’s Child is <PID_of_Child>`
- **Child prints:**
  - `Child is having ID <PID>`
  - `My Parent ID is <PID_of_Parent>`
- **Required order of output:** A, C, D, B (see above).
- Use `wait()` to enforce this order.

### Additional Task

- **Using `vfork()`/`fork()`:**
  - Child computes the factorial of 4.
  - Parent computes and prints the first 16 Fibonacci numbers.
  - Parent waits for the child to finish.
- **Bonus:** Child waits for the parent to finish before executing.

---

## Files

- [`Q1.c`](Q1.c): Main source code implementing the parent-child relationship, synchronization, Fibonacci and factorial calculations.
- [`Makefile`](Makefile): For compiling and executing the code.

---

## How It Works

- The code uses `fork()` to create a child process.
- Parent and child print their respective PIDs and compute their respective sequences.
- Synchronization is achieved using `wait()` so that the required order of output is maintained.
- Error handling is present for system call failures (`fork()`).

---

## Compilation and Execution

To compile and run the code, use the provided Makefile:

```sh
make
```

This will:
- Compile `Q1.c` into an executable named `1`.
- Run the executable, displaying the required outputs.

---

## Sample Output

```
Parent process printing Fibonacci Series: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610
Child process will finish after parent.
Child Process computing Factorial of 4: 24
```

---

## Error Handling

- If `fork()` fails, an error message is printed using `perror()`, and the program exits gracefully.
- All system calls are checked for success.

---

## Brief Description

This program demonstrates basic OS concepts:
- **Process Creation:** Using `fork()`, two processes are created.
- **Process Synchronization:** `wait()` ensures correct output order and parent-child coordination.
- **Computation:** Parent calculates Fibonacci series, child calculates factorial.
- **Resource Management:** Proper waiting prevents zombies and race conditions.

---

## Author

- [Ayush-0404](https://github.com/Ayush-0404)

---

> For any questions or issues, feel free to open an issue in the main repository.

---
