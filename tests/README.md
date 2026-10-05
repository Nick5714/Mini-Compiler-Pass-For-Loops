# Tests

This folder contains small C examples that can be compiled to LLVM IR and passed through the optimizer.

Example workflow:

1. Compile a C file to LLVM IR with clang.
2. Run the `loop-pass` tool on the emitted `.ll` file.
3. Compare the before/after IR using `scripts/compare_ir.py`.
