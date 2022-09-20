---
title: "if else pattern"
parmalink: /if_else
layout: default
---
# If Else Pattern
# Context
This Pattern requires:
* Context Pattern
* Basic Block Pattern
# Solution
```cpp
void create_if_else(llvm::Function* func, llvm::Value* cmp) {

	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", func);
	llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", func);

	builder.CreateCondBr(cmp, ifBlock, elseBlock);

	builder.SetInsertPoint(ifBlock);
	//if block logic goes here

	builder.SetInsertPoint(elseBlock);
	//else block logic goes here

}
```
# Example
# Variations
```cpp
template<typename cFunc, typename iFunc, typename eFunc>
llvm::PHINode* create_if_else(llvm::Function* func, llvm::Type* blockType, cFunc cmp_code, iFunc if_code, eFunc else_code) {
	llvm::Value* cmp = cmp_code(func, blockType);
	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", func);
	llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", func);
	llvm::BasicBlock* together = llvm::BasicBlock::Create(context, "together", func);

	builder.CreateCondBr(cmp, ifBlock, elseBlock);

	builder.SetInsertPoint(ifBlock);
	llvm::Value* ifV = if_code(func, blockType);
	builder.CreateBr(together);

	builder.SetInsertPoint(elseBlock);
	llvm::Value* elseV = else_code(func, blockType);
	builder.CreateBr(together);
	llvm::PHINode *PN = Builder.CreatePHI(blockType, 2, "iftmp");

	PN->addIncoming(ifV, ifBlock);
	PN->addIncoming(elseV, elseBlock);

	builder.SetInsertPoint(together);
	builder.CreateRet(PN);
	return PN;
}
```
# Related patterns
# References