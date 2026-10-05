#include "LoopOptimizationPass.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace mini {

namespace {

static StringRef getBlockNameOrUnnamed(const BasicBlock *BB) {
  if (BB == nullptr) {
    return "<null>";
  }
  if (BB->hasName()) {
    return BB->getName();
  }
  return "<unnamed>";
}

} // namespace

void LoopOptimizationPass::printLoopSummary(Loop &L, unsigned Depth) const {
  std::string Indent(Depth * 2, ' ');
  BasicBlock *Header = L.getHeader();
  BasicBlock *Latch = L.getLoopLatch();

  errs() << Indent << "[loop] header=" << getBlockNameOrUnnamed(Header)
         << ", latch=" << getBlockNameOrUnnamed(Latch)
         << ", blocks=" << L.getBlocks().size() << '\n';
}

bool LoopOptimizationPass::simplifyLoopBody(Loop &L) {
  SmallVector<Instruction *, 8> Candidates;

  for (BasicBlock *BB : L.blocks()) {
    for (Instruction &I : *BB) {
      if (I.getOpcode() != Instruction::Mul) {
        continue;
      }

      auto *BO = dyn_cast<BinaryOperator>(&I);
      if (BO == nullptr || !BO->getType()->isIntegerTy()) {
        continue;
      }

      Value *LHS = BO->getOperand(0);
      Value *RHS = BO->getOperand(1);
      auto *LHSC = dyn_cast<ConstantInt>(LHS);
      auto *RHSC = dyn_cast<ConstantInt>(RHS);

      const bool IsMulByTwo = (LHSC != nullptr && LHSC->equalsInt(2)) ||
                              (RHSC != nullptr && RHSC->equalsInt(2));
      if (!IsMulByTwo) {
        continue;
      }

      Candidates.push_back(BO);
    }
  }

  bool Changed = false;
  for (Instruction *I : Candidates) {
    auto *BO = cast<BinaryOperator>(I);
    Value *LHS = BO->getOperand(0);
    Value *RHS = BO->getOperand(1);

    Value *OtherOp = nullptr;
    if (auto *ConstLHS = dyn_cast<ConstantInt>(LHS); ConstLHS != nullptr && ConstLHS->equalsInt(2)) {
      OtherOp = RHS;
    } else if (auto *ConstRHS = dyn_cast<ConstantInt>(RHS); ConstRHS != nullptr && ConstRHS->equalsInt(2)) {
      OtherOp = LHS;
    }

    if (OtherOp == nullptr) {
      continue;
    }

    IRBuilder<> Builder(BO);
    Value *ShiftAmount = ConstantInt::get(BO->getType(), 1);
    Value *Replacement = Builder.CreateShl(OtherOp, ShiftAmount, "strength.reduced");

    BO->replaceAllUsesWith(Replacement);
    BO->eraseFromParent();
    Changed = true;
  }

  return Changed;
}

bool LoopOptimizationPass::processLoop(Loop &L, unsigned Depth) {
  bool Changed = false;

  printLoopSummary(L, Depth);
  Changed |= simplifyLoopBody(L);

  for (Loop *SubLoop : L.getSubLoops()) {
    if (SubLoop != nullptr) {
      Changed |= processLoop(*SubLoop, Depth + 1);
    }
  }

  return Changed;
}

PreservedAnalyses LoopOptimizationPass::run(Function &F,
                                            FunctionAnalysisManager &AM) {
  if (F.isDeclaration()) {
    return PreservedAnalyses::all();
  }

  auto &LI = AM.getResult<LoopAnalysis>(F);
  if (LI.empty()) {
    return PreservedAnalyses::all();
  }

  errs() << "[function] " << F.getName() << '\n';

  bool Changed = false;
  for (Loop *TopLevelLoop : LI) {
    if (TopLevelLoop != nullptr) {
      Changed |= processLoop(*TopLevelLoop, 1);
    }
  }

  if (Changed) {
    return PreservedAnalyses::none();
  }

  return PreservedAnalyses::all();
}

} // namespace mini
