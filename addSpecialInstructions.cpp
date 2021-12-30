void VPlan::addSpecialInstructions(VPTransformState *State) {
  auto VF = State->VF;
  BasicBlock *VectorPreHeaderBB = State->CFG.VectorPreHeader;
  BasicBlock *newBB = VectorPreHeaderBB->splitBasicBlock(&VectorPreHeaderBB->back(), VectorPreHeaderBB->getName() + ".split", true);
  LLVMContext &Context = VectorPreHeaderBB->getContext();
  BasicBlock *whileBodyBB = BasicBlock::Create(Context, "while.body", VectorPreHeaderBB->getParent(), VectorPreHeaderBB);
  State->Builder.SetInsertPoint(whileBodyBB);
  
  std::string SysRegStr = "3:0:15:15:0";
  Metadata *Ops[] = { llvm::MDString::get(Context, SysRegStr) };
  MDNode *RegName = llvm::MDNode::get(Context, Ops);
  Value *Metadata = llvm::MetadataAsValue::get(Context, RegName);

  Type *RegisterType = Type::getInt64Ty(Context);
  Type *Types[] = { RegisterType };
  Function *FWrite = Intrinsic::getDeclaration(VectorPreHeaderBB->getModule(), Intrinsic::write_register, Types);
  FunctionCallee calleeWrite(FWrite);
  ArrayRef<Value *> WriteArgs = { Metadata , ConstantInt::get(RegisterType, VF.getKnownMinValue()) };
  State->Builder.CreateCall(calleeWrite, WriteArgs);
  Function *FRead = Intrinsic::getDeclaration(VectorPreHeaderBB->getModule(), Intrinsic::read_register, Types);
  FunctionCallee calleeRead(FRead);
  ArrayRef<Value *> ReadArgs = { Metadata };
  CallInst *callInst = State->Builder.CreateCall(calleeRead, ReadArgs);
  Value *cmp = State->Builder.CreateICmpEQ(callInst, ConstantInt::get(RegisterType, VF.getKnownMinValue()));
  State->Builder.CreateCondBr(cmp, VectorPreHeaderBB, whileBodyBB);
  //splitBB -> whileBodyBB -> VectorPreHeaderBB
  if (BranchInst *brInst =  dyn_cast<BranchInst>(&newBB->back())) {
      brInst->setSuccessor(0, whileBodyBB);
  }
  State->DT->recalculate(*VectorPreHeaderBB->getParent());
}
