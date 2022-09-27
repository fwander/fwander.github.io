---
title: "attributes"
layout: default
---

# Attributes
Attributes are used when we want to pass information to the llvm backend about a function.
They can be used to tell the backend that a function either has or lacks certain properties.

if we have a `llvm::Function*` called `func` we can change it's attributes like so:

```cpp
func->addAttribute(0,llvm::Attribute::AlwaysInline);
```

This will tell the compiler to always inline func.

# Varieties
Here are a list of `llvm::Attributes`.

[Attribute Namespace Reference](http://formalverification.cs.utah.edu/llvm_doxy/2.9/namespacellvm_1_1Attribute.html#ad722c8fd66d5b83af678f3ebfb556634)

