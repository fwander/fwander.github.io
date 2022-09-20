---
title: "context"
parmalink: /context
layout: default
---
# Context and Module Pattern
# Context
# Solution
For small projects it is fine to put the context, module and irbuilder in static global variables.
```c
static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("<name of module>", context);
static llvm::IRBuilder<> builder(context);
```
# Example
# Variations
# Related patterns
# References