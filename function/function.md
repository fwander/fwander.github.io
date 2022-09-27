---
title: "function pattern"
layout: default
---
# Function Pattern
# Intent
* Emit LLVM IR with a callable function
* Emit LLVM IR that is contained within this function

# Additional Features
* Specify the return type of this function
* Specify the types of the function arguments

# Context
This Pattern requires:
* Type Pattern
* Basic Block Pattern

# Structure
A function in LLVM can't just be a declaration followed by instructions. It must have at least one Basic Block to serve as an insert point for execution for that function.
That's why, when we setup a function, we almost always want to create a basic block to serve this purpose and then tell our IrBuilder to start adding instructions after the insert point.
This pattern has only two parts. Setup and Return

Setup goes as follows:

```cpp
llvm::Function* <new_function> = llvm::Function::Create(  
	llvm::FunctionType::get(builder.getInt32Ty(), false), 
	llvm::Function::ExternalLinkage, 
	"<name>",
	module
);
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", <new_function>); 
builder.SetInsertPoint(entryPoint); 
```
where 
`new_function` is the name of the variable to store the llvm::Function* in.
`name` is the name of the function as you want it to appear in LLVM IR

Return is simply:

```cpp
builder.CreateRet(builder.getInt32(0)); 
```

The contents of your function would go between Setup and Return.

## Example

```cpp
llvm::Function* func = llvm::Function::Create(  
	llvm::FunctionType::get(builder.getInt32ty(), false), 
	llvm::Function::ExternalLinkage, 
	"fib",
	module
);
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", func); 
builder.SetInsertPoint(entryPoint); 
//contents goes here in the form of calls on the builder object because it is set up to insert instructions in the entry basic block of our function
builder.CreateRet(builder.getInt32(0)); 
```

# Additions

In order to change the return type you must change the type stored in the <new_function> variable. Additionally you must change the variable returned by the instruction created by `builder.CreateRet()` call.

New Setup:

```cpp
llvm::Function* <new_function> = llvm::Function::Create(  
	llvm::FunctionType::get(builder.get<return_type>(), false), 
	llvm::Function::ExternalLinkage, 
	"<name>",
	module
);
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", <new_function>); 
builder.SetInsertPoint(entryPoint); 
```

## Example

```cpp
llvm::Function* func = llvm::Function::Create(  
	llvm::FunctionType::get(builder.getInt8ty(), false), 
	llvm::Function::ExternalLinkage, 
	"fib",
	module
);
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", func); 
builder.SetInsertPoint(entryPoint); 
builder.CreateRet(builder.getInt8(0)); 
```

The second addition we can make just adds the parameter types to the function.

``` cpp
llvm::Function* <new_function> = llvm::Function::Create(  
	llvm::FunctionType::get(<return_type>, <arg_types>, false), 
	llvm::Function::ExternalLinkage, 
	"<name>",
	module
);
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", <new_function>); 
builder.SetInsertPoint(entryPoint); 
```

Where
`arg_types` is an ArrayRef containing the llvm::Type types of the function parameters.

# Variations

``` cpp
template<typename Func>
llvm::Function* create_function(std::string name, std::vector<std::string> argNames, std::vector<llvm::Type*> argTypes, llvm::Type* retType, Func body) {
	llvm::Function* func = llvm::Function::Create(  
		llvm::FunctionType::get(retType, argTypes, false), 
		llvm::Function::ExternalLinkage, 
		name,
		module
	);
	for (unsigned i; i < func->arg_size(); i = 0){
		func->getArg(i)->setName(argNames[i]);
	}
	llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", func); 
	builder.SetInsertPoint(entryPoint); 
	llvm::Value* ret = body(func, retType);
	builder.CreateRet(ret)
	return func;
}
```

# Related patterns
* Inline only Function
# References