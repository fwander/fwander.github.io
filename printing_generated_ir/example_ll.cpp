#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <vector>

static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("llvm design patterns", context);
static llvm::IRBuilder<> builder(context);

int main(int argc, char *argv[]) {
	std::string s;
	llvm::raw_string_ostream os {s};
	module->print(os, nullptr);
	std::cout << os.str() << std::endl;
	return 0;
}
