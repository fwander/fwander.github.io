#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <vector>

static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("llvm design patterns", context);
static llvm::IRBuilder<> builder(context);

llvm::Function* create_function(std::string name, std::vector<std::string> argNames, std::vector<llvm::Type*> argTypes, llvm::Type* retType) {
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
	//function body goes here
	//builder.CreateRet(); 
	return func;
}

int main(int argc, char *argv[]) {
	create_function("fib", {"a"}, {builder.getInt32Ty()}, builder.getInt32Ty());

	std::string s;
	llvm::raw_string_ostream os {s};
	module->print(os, nullptr);
	std::cout << os.str() << std::endl;
	return 0;
}
