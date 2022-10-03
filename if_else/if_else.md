---
title: "if else pattern"
layout: default
---
# If Else Pattern
## Problem
With this pattern we want to emit LLVM IR that conditionally branches between two pieces of code.
	
## Use
Basic Block can only execute instructions sequentially. They don't have to ability to conditionally branch. The If Else Pattern allows for these conditional branches.

## Context
This pattern should be used when you need to generate LLVM IR that branches based on a condition
```C
if(condition){
	// if block
}
else {
	// else block
}
```

# Participating Patterns
This Pattern requires:
* Context Pattern
* Basic Block Pattern
* Boolean Expressions

# Solution
To do this we need to create 3 new Basic Blocks. One for the code to execute if the condition we are branching on is true, one for false, and one last one to put code that goes after the if else blocks.
In order for a Basic Block to be correctly formed, it must consist of non terminating instructions followed by one terminating instruction. This means we can't just let the else block "fall" through to the together block.

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

In this case, the `llvm::Value*` cmp is the value the code branch hinges on. It must be generated using boolean expressions in IR.

## Example

Let's say you wanted to generate the following code

```C
int equals(val1, val2){
	if (val1 == val2){
		return 1;
	}
	else{
		return 0;
	}
}
```

Let's say there is a well formed `llvm::Function*` called func that we want to place the if statement inside of. We can also assume `val1` and `val2` are `llvm::Value*`s that are defined as the function's parameters.

```cpp
llvm::Value* cmp = builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_EQ,val1, val2);
llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", func);
llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", func);
llvm::BasicBlock* together = llvm::BasicBlock::Create(context, "together", func);

builder.CreateCondBr(cmp, ifBlock, elseBlock);

builder.SetInsertPoint(ifBlock);
builder.CreateRet(builder.getInt32(1));
builder.CreateBr(together);

builder.SetInsertPoint(elseBlock);
builder.CreateRet(builder.getInt32(0));
builder.CreateBr(together);

builder.SetInsertPoint(together); // The code should never get here.
builder.CreateRet(builder.getInt32(0));
```

Here, the pattern has redundancy because we don't need the together block as we return from both Basic Blocks.

# Variations

## PHINode
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

## Example

Let's do the example from earlier again except the code we want is:

```C
int equals(val1, val2){
	int value;
	if (val1 == val2){
		value = 1;
	}
	else{
		value = 0;
	}
	return value;
}
```

```cpp
llvm::Value* cmp = builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_EQ,val1, val2);
llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", func);
llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", func);
llvm::BasicBlock* together = llvm::BasicBlock::Create(context, "together", func);

builder.CreateCondBr(cmp, ifBlock, elseBlock);

builder.SetInsertPoint(ifBlock);
llvm::Value* ifV = builder.getInt32(1);
builder.CreateBr(together);

builder.SetInsertPoint(elseBlock);
llvm::Value* elseV = builder.getInt32(0);
builder.CreateBr(together);

builder.SetInsertPoint(together);

llvm::PHINode *PN = Builder.CreatePHI(blockType, 2, "iftmp");
PN->addIncoming(ifV, ifBlock);
PN->addIncoming(elseV, elseBlock);

builder.CreateRet(PN);
```

## If Statement

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