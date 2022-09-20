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
```c
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
# Related patterns
# References