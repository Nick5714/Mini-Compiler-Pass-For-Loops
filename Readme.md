# Mini Compiler Pass: Loop Optimizations

This project is a small LLVM middle-end pass that demonstrates loop analysis and simple loop optimizations on LLVM IR.

## Goal

Build a standalone LLVM-based tool that:

- reads LLVM IR from a `.ll` file,
- finds natural loops,
- applies a simplified loop unrolling transformation,
- applies a simplified strength-reduction transformation,
- writes transformed IR back out for inspection.

## Project structure

```text
Mini-Compiler-Pass-Loops/
├── CMakeLists.txt
├── README.md
├── include/
│   └── LoopOptimizationPass.h
├── src/
│   ├── Main.cpp
│   └── LoopOptimizationPass.cpp
├── tests/
│   ├── sum_array.c
│   └── expected/
└── scripts/
	└── compare_ir.py
```


## Current implementation

The scaffold now includes:

- a CMake build file,
- a standalone LLVM IR driver,
- a new-pass-manager function pass,
- loop discovery and loop summary logging,
- a conservative strength-reduction example for `mul` by `2`,
- a sample test C file,
- a small IR comparison script.

## Build and run

1. Configure the project with CMake.
2. Build the `loop-pass` executable.
3. Compile a test C file to LLVM IR with `clang`.
4. Run `loop-pass` on the `.ll` file and inspect the output IR.
5. Use `scripts/compare_ir.py` to compare instruction counts.
