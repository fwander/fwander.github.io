---
title: "if else pattern"
layout: default
---
# If Else Pattern
## Problem
	With this pattern we want to emit LLVM IR that conditionally branches between two pieces of code.
	
## Use
	basic block can only execute instructions sequentially. They don't have to ability to conditionally branch. The If Else Pattern allows for these conditional branches.

# Context
This Pattern requires:
* Context Pattern
* basic block Pattern

# Solution
	To do this we need to create 3 new basic blocks. One for the code to execute if the condition we are branching on is true, one for false, and one last one to put code that goes after the if else blocks.
	In order for a basic block to be correctly formed, it must consist of non terminating instructions followed by one terminating instruction. This means we can't just let the else block "fall" through to the together block.

```cpp
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
	builder.CreateBr(together);

	builder.SetInsertPoint(together);
}
```

# Example
# Variations
One variation to the solution is a templated function that takes in 3 lambda functions. These functions should contain code for generating IR in the correct places (comparison, if block, and else block). 
A drawback of this is that is shoehorns you into a coding style where the if and else blocks must have the same type as the type of the whole if else block. If this is not something you want, this variation is not the correct solution

``` cpp
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

``` cpp
void create_if_else(llvm::Function* func, llvm::Value* cmp) {
	llvm::BasicBlock* ifBlock  = llvm::BasicBlock::Create(context, "ifBlock" , func);
	llvm::BasicBlock* together = llvm::BasicBlock::Create(context, "together", func);

	builder.CreateCondBr(cmp, ifBlock, together);

	builder.SetInsertPoint(ifBlock);
	//if block logic goes here
	builder.CreateBr(together)

	builder.SetInsertPoint(together);
	//Code returns to normal

}
```

# References