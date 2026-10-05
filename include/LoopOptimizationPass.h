#pragma once

#include "llvm/IR/PassManager.h"

namespace llvm {
class Function;
class FunctionAnalysisManager;
class Loop;
} // namespace llvm

namespace mini {

class LoopOptimizationPass : public llvm::PassInfoMixin<LoopOptimizationPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);

private:
  bool processLoop(llvm::Loop &L, unsigned Depth);
  bool simplifyLoopBody(llvm::Loop &L);
  void printLoopSummary(llvm::Loop &L, unsigned Depth) const;
};

} // namespace mini
