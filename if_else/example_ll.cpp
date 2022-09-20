#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <vector>

static llvm::LLVMContext context;
static llvm::Module *module= new llvm::Module("llvm design patterns", context);
static llvm::IRBuilder<> builder(context);

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

int main(int argc, char *argv[]) {
	auto fn = create_function<>("fib", {"a"}, {builder.getInt32Ty()}, builder.getInt32Ty(), 
		[](llvm::Function* func, llvm::Type* blockType) {
			return create_if_else<>(func, blockType,
				[](llvm::Function* func, llvm::Type* blockType){
					return builder.getInt32(0);
				},
				[](llvm::Function* func, llvm::Type* blockType){
					return builder.getInt32(0);
				},
				[](llvm::Function* func, llvm::Type* blockType){
					return builder.getInt32(1);
				}
			);
		}
	);

	std::string s;
	llvm::raw_string_ostream os {s};
	module->print(os, nullptr);
	std::cout << os.str() << std::endl;
	return 0;
}
