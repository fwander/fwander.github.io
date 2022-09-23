---
title: "if else pattern"
layout: default
---
# If Else Pattern
## Problem
	The problem the If Else Pattern seeks to solve is as follows. How can you create LLVM IR that executes one basic block when a boolean expression is True and another basic block otherwise.
	
## Use
	Basic Blocks can only execute instructions sequentially. They don't have to ability to conditionally branch. The If Else Pattern allows for these conditional branches.

# Context
This Pattern requires:
* Context Pattern
* Basic Block Pattern

# Solution

The solution is to create two basic blocks, ask the irbuilder to create a conditional branch with the two basic block variables as input. After that is set up you can create the insert points for both basic blocks. 

```
cpp
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

There is a problem with this solution though; if the code is executing and jumps to the ifBlock, if there is no jump statement in the ifBlock, execution will continue with the elseBlock.
This is not the behavior that is implied by "if else."

```
cpp
void create_if_else(llvm::Function* func, llvm::Value* cmp) {
	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", func);
	llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", func);
	llvm::BasicBlock* together = llvm::BasicBlock::Create(context, "together", func);

	builder.CreateCondBr(cmp, ifBlock, elseBlock);

	builder.SetInsertPoint(ifBlock);
	//if block logic goes here
	builder.CreateBr(together);

	builder.SetInsertPoint(elseBlock);
	//else block logic goes here

	builder.SetInsertPoint(together);
}
```

# Example
# Variations
One variation to the solution is a templated function that takes in 3 lambda functions. These functions should contain code for generating IR in the correct places (comparison, if block, and else block). 
A drawback of this is that is shoehorns you into a coding style where the if and else blocks must have the same type as the type of the whole if else block. If this is not something you want, this variation is not the correct solution

```
cpp
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
It is also possible to create just an if block.

```
cpp
void create_if_else(llvm::Function* func, llvm::Value* cmp) {
	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "ifBlock", func);
	llvm::BasicBlock* skip = llvm::BasicBlock::Create(context, "skip", func);

	builder.CreateCondBr(cmp, ifBlock, skip);

	builder.SetInsertPoint(ifBlock);
	//if block logic goes here

	builder.SetInsertPoint(skip);
	//Code returns to normal

}
```

This is identical code to the original if else pattern that was "wrong." Here it is correct because of a change of perspective.

# References