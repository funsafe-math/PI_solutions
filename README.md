[![CMake](https://github.com/tadeuszk733/PI_solutions/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/tadeuszk733/PI_solutions/actions/workflows/cmake.yml)

# Solutions to Imperative Programming
## Features
- Auto-generate test cases
- Test cases can be run in debugger

## Example usage:
### Assume you have work file `vec.pdf` 
- Put the `vec.pdf` file into the `zadania/` directory
- Put your solution file `vec-sol.c` into the `src/` directory
- Generate test cases - run the provided python script `python generate_testcases.py zadania/vec.pdf vec`
- Auto-test using CTest 
  (Just run `ctest --build-and-test` in the `build` directory) 
- Optionally you can place the `vec-template.c` file in the `template` directory

### Before posting questions, look at the `vec.c` example 
---
## Requirements
1. Linux
2. `pdfgrep` package installed
3. `cmake` and a recent `C/C++` compiler

