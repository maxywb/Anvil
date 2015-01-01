#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <memory>

/*
int32 function foo() {
  x = 0;
  i = 0;
  for(; i < 10; i++) {
    x = x + 1;
  }
  return x;
}
*/

#define LlvmInt32(builder, value) (builder.getInt32(value))

#define FunctionInputTerminator ((llvm::Type *)0)

int main() {
  
    llvm::InitializeNativeTarget();

    llvm::LLVMContext context;
    llvm::Type * int32Type = llvm::Type::getInt32Ty(context);

    // make a module to hold the function
    std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("test", context);

    // setup "main" function and top level basic block
    llvm::Function *mainFunction =
        llvm::cast<llvm::Function>(module->getOrInsertFunction("main", llvm::Type::getInt32Ty(context),
							       FunctionInputTerminator));
    
    
    // initialize builders
    llvm::BasicBlock * setupBlock = llvm::BasicBlock::Create(context, "setup", mainFunction);
    llvm::IRBuilder<> setup(setupBlock);
    llvm::BasicBlock * conditionBlock = llvm::BasicBlock::Create(context, "condition", mainFunction);
    llvm::IRBuilder<> condition(conditionBlock);
    llvm::BasicBlock * bodyBlock = llvm::BasicBlock::Create(context, "body", mainFunction);
    llvm::IRBuilder<> body(bodyBlock);
    llvm::BasicBlock * postBlock = llvm::BasicBlock::Create(context, "post", mainFunction);
    llvm::IRBuilder<> post(postBlock);



    /* setup */
    // initialize x to 0
    llvm::AllocaInst * xLocation = setup.CreateAlloca(int32Type,0,"xStore");
    setup.CreateStore(LlvmInt32(setup,0),xLocation,"xStore");

    // initialize i to 0
    llvm::AllocaInst * iLocation = setup.CreateAlloca(int32Type,0,"iStore");
    setup.CreateStore(LlvmInt32(setup,0),iLocation,"iStore");

    setup.CreateBr(conditionBlock);

    /* condition */
    llvm::Value * iValue = condition.CreateLoad(iLocation,"iValue");
    llvm::Value * conditionValue = condition.CreateSub(LlvmInt32(condition,10),iValue);

    conditionValue = condition.CreateICmpNE(conditionValue,LlvmInt32(condition,0));

    // increment i
    iValue = condition.CreateAdd(iValue,LlvmInt32(condition,1));
    condition.CreateStore(iValue,iLocation,"iStore");

    condition.CreateCondBr(conditionValue,bodyBlock,postBlock);

    /* body */
    // x = x + 1
    llvm::Value * xValue = body.CreateLoad(xLocation,"xValue");
    xValue = body.CreateAdd(xValue, LlvmInt32(body,1));
    body.CreateStore(xValue,xLocation,"xStore");

    body.CreateBr(conditionBlock);

    /* post */
    llvm::Value * returnValue = post.CreateLoad(xLocation,"xValue");
    post.CreateRet(returnValue);
    
    // dump to stdout and execute
    llvm::outs() << *module.get();
    llvm::outs().flush();

    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(module)).create();

    // Call the `main' function with no arguments:
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue gv = EE->runFunction(mainFunction, noargs);

    // Import result of execution:
    llvm::outs() << "Result: " << gv.IntVal << "\n";
    EE->freeMachineCodeForFunction(mainFunction);

    delete EE;
    llvm::llvm_shutdown();
    return 0;
}
