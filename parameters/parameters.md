---
title: "function pattern"
layout: default
---

# Parameters
The Parameter pattern should be used to add parameters to functions emitted to LLVM IR.

The number and type of parameters in a function declaration are defined in the `llvm::FunctionType` object given to `llvm::Function::create`

# Obstacles
While it is easy to compile a list of `llvm::Type` objects for the parameters of a function, there are aspects to parameters that can get pretty tricky. Default values and variadic parameters are both non trivial.

# Context
This pattern applies to the following examples.

```Cpp
int foo(int a, char b){
    return 0;
}

int foobar(int a, int b){
    return a + b;
}

int bar(int a = 0){
    return a;
}

```

This examples cover declaring the parameters of a function, default values, and accessing these parameters.

# Defining Parameters

We define the parameters a function takes in by changing the `llvm::FunctionType`.

```Cpp
auto functionType = llvm::FunctionType::get(<return_type>, {builder.getInt32ty(), builder.getInt8ty()} , false);
llvm::Function* foo = llvm::Function::Create(  
    functionType,
	llvm::Function::ExternalLinkage, 
	"foo",
	module
);
```

# Accessing Parameters

Let's say there's a `llvm::Function*` created called foobar which matches the declaration of `foobar` above. To access it's variables (and fill out the function body) we can do the following.

```Cpp
llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", foobar); 
builder.SetInsertPoint(entryPoint); 

llvm::Value* a= foobar->getArg(0);
llvm::Value* b= foobar->getArg(1);

llvm::Value* result = builder.CreateAdd(a,b,"addres");

builder.CreateRet(result); 
```

Here, `getArg()` was used to access an `llvm::Value*` to the nth argument of foobar.

Instead of using `getArg()` you can call `arg_begin()`, `arg_end()`, and `args()` on an `llvm:Function` to get an iterator to the beginning and end of the arguments, or an iterator range respectively.

# Default Parameters

LLVM does not support adding default parameters into the IR, however, you can still get the same behavior for a language you are designing by generating the correct IR. To get default parameters you need to store their values with function declaration as you are generating code. After the IR is fully generated for a function with defaults, however, the function declaration would look no different if there were no default parameters.

The way you get the behavior of default parameters is by ensuring that every time you make a function call with fewer parameters you fill in the default values into the call.

For example, the following C++ code

```Cpp
bar(0);
bar();
```

would compile to two equivalent LLVM IR function calls.
