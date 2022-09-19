# Function Pattern
# Context
# Solution
```
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
```
# Example
# Variations
# Related patterns
# References