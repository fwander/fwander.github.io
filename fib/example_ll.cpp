#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <vector>

static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("llvm design patterns", context);

void create_fib_example() {
	static llvm::IRBuilder<> builder(context);

	static std::vector<std::string> argNames{"a"}; //part of pattern 1
	static std::vector<llvm::Type*> argTypes{llvm::Type::getInt32Ty(context)}; //part of pattern 1

	llvm::Function* fib = llvm::Function::Create(  //pattern 1: function
		llvm::FunctionType::get(builder.getInt32Ty(), argTypes,false), //pattern 2: types
		llvm::Function::ExternalLinkage, //ways to link functions should be discussed 
		"fib",
		module
	);

	llvm::Value* arg = fib->arg_begin(); // part of pattern 1
	arg->setName(argNames[0]); //part of pattern 1 TODO make general
	
	llvm::BasicBlock* entryPoint = llvm::BasicBlock::Create(context, "entry", fib); //pattern 3: Blocks < pattern 1
	builder.SetInsertPoint(entryPoint); //part of pattern 3

	llvm::Value* cmp = builder.CreateICmpULE(arg,builder.getInt32(1), "cmpres");

	llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if", fib);
	llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(context, "else", fib);

	builder.CreateCondBr(cmp, ifBlock, elseBlock);

	builder.SetInsertPoint(ifBlock);
	builder.CreateRet(builder.getInt32(1));

	builder.SetInsertPoint(elseBlock);
	llvm::Value* minus1 = builder.CreateSub(arg,builder.getInt32(1),"minusres");
	llvm::Value* minus2 = builder.CreateSub(arg,builder.getInt32(2),"minusres");
	llvm::Value* call1 = builder.CreateCall(fib,minus1);
	llvm::Value* call2 = builder.CreateCall(fib,minus2);
	llvm::Value* result = builder.CreateAdd(call1,call2,"addres");

	builder.CreateRet(result); //part of pattern 1
}

// create module (setup pattern): context and module
// print code with stream

int main(int argc, char *argv[]) {
	create_fib_example();

	std::string s;
	llvm::raw_string_ostream os {s};
	module->print(os, nullptr);
	std::cout << os.str() << std::endl;
	return 0;
}
