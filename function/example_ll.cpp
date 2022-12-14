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
	func->addAttribute(0,llvm::Attribute::AlwaysInline);
	for (unsigned i; i < func->arg_size(); i++){
		func->getArg(i)->setName(argNames[i]);
	}
	llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", func); 
	builder.SetInsertPoint(entryPoint); 
	//function body goes here
	builder.CreateRet(builder.getInt32(0)); 
	return func;
}

int main(int argc, char *argv[]) {
	create_function("fib", {"a"}, {builder.getInt32Ty()}, builder.getInt32Ty());

	module->print(llvm::outs(), nullptr);
	return 0;
}
