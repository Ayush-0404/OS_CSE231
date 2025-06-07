

---

# Bash Script Shell - Arithmetic Calculator

## Overview

This project implements a Bash-based arithmetic calculator designed for school use. It processes mathematical operations specified in an `input.txt` file, performs calculations, and then writes the results to an `output.txt` file inside a `Result` directory. The script ensures the `Result` directory exists, creating it if necessary.

**Supported Operations:**
- `xor`: Computes the bitwise XOR of two numbers.
- `product`: Computes the product of two numbers.
- `compare`: Returns the bigger number of the two.

## Features & Rubric Checklist

- **Program Source Code(s) with Makefile**: All scripts are included, with a Makefile to manage running and cleaning up.
- **Brief Write-up**: This file (README.md) explains the project, workflow, and file structure.
- **File Handling in Bash**: All file reading/writing is handled using Bash scripting.
- **Directory Creation in Bash**: The script creates the `Result` directory if it doesn't exist.

## How It Works

1. **Input Format**  
   The `input.txt` file must be present in the working directory, with each line in the format:
   ```
   x y operation
   ```
   - `x` and `y` are integers.
   - `operation` is one of: `xor`, `product`, `compare`.

   **Example:**
   ```
   5 3 xor
   6 7 product
   12 4 compare
   ```

2. **Execution**
   - Run the main script using the Makefile or directly via terminal.
   - The script reads each line from `input.txt`, performs the specified operation, and writes the result (one per line) to `Result/output.txt`.
   - If the `Result` directory does not exist, it will be created automatically.

3. **Output**
   - Results are saved in `Result/output.txt`, with each result corresponding to each input line.

## Usage

### 1. Place your `input.txt` in the folder.

### 2. Run the Calculator

**Using the Makefile:**
```bash
make run
```

**Directly (if script is named `calculator.sh`):**
```bash
bash calculator.sh
```

### 3. View Results

- Output will be in `Result/output.txt`.

### 4. Clean Up (Optional)

- To remove the output files and created directory:
```bash
make clean
```

## File Structure

```
Bash script shell - Arithmetic Calculator/
│
├── calculator.sh       # Main Bash script
├── input.txt           # Input file with calculation requests
├── Makefile            # For running and cleaning
├── README.md           # Project description
└── Result/
    └── output.txt      # Generated results (created by script)
```

## Example

**input.txt**
```
8 3 xor
4 5 product
10 15 compare
```

**output.txt**
```
11
20
15
```


## Screenshots

Below are some screenshots demonstrating the working of the calculator script:

![Calculator Working - Step 1](Screenshot%202025-06-07%20060951.png)
![Calculator Working - Step 2](Screenshot%202025-06-07%20061010.png)
![Calculator Working - Step 2](Screenshot%202025-06-07%20061026.png)
![Calculator Working - Step 2](Screenshot%202025-06-07%20061038.png)

<!-- Add more screenshots as needed -->

## Notes

- Ensure `input.txt` is formatted as specified.
- Only the three specified operations (`xor`, `product`, `compare`) are supported.
- The script is written purely in Bash and uses standard utilities.

## Notes

- Ensure `input.txt` is formatted as specified.
- Only the three specified operations (`xor`, `product`, `compare`) are supported.
- The script is written purely in Bash and uses standard utilities.

---

End of README

---
