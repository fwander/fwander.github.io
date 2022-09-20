#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <vector>

static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("llvm design patterns", context);
static llvm::IRBuilder<> builder(context);
