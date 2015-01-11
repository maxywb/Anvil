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
#include <vector>

/*
int32 function foo(int32 index) {
  x = malloc(length*sizeof(int32));
  x[0] = 2;
  i = 1;
  for(; i < 10; i++) {
    x[i] = x[i-1] * 2;
  }
  return x[index];
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
        llvm::cast<llvm::Function>(module->getOrInsertFunction("foo", 
							       llvm::Type::getInt32Ty(context), // output
							       llvm::Type::getInt32Ty(context), // input
							       FunctionInputTerminator));
    

    // setup malloc function
    llvm::PointerType * outputType = llvm::PointerType::get(llvm::Type::getVoidTy(context),0);
    std::vector<llvm::Type *> params(1,llvm::Type::getInt32Ty(context));
    llvm::FunctionType *mallocType = llvm::FunctionType::get(outputType,params,false);
    llvm::Function *mallocFunction = llvm::cast<llvm::Function>(module->getOrInsertFunction("malloc",mallocType));

    
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
    // array = int32[10]
    llvm::CallInst * mallocResult = setup.CreateCall(mallocFunction, LlvmInt32(post,10*4));

    llvm::Value * array = setup.CreatePointerCast(mallocResult,
						  llvm::PointerType::get(llvm::Type::getInt32Ty(context),0),"array");
    setup.CreateStore(LlvmInt32(setup,2), array, "array[0]=2");

    // i = 1
    llvm::AllocaInst * iLocation = setup.CreateAlloca(int32Type,0,"iStore");
    setup.CreateStore(LlvmInt32(setup,1),iLocation,"iStore");

    setup.CreateBr(conditionBlock);

    /* condition */
    llvm::Value * iValue = condition.CreateLoad(iLocation,"iValue");
    llvm::Value * conditionValue = condition.CreateSub(LlvmInt32(condition,10),iValue);

    conditionValue = condition.CreateICmpNE(conditionValue,LlvmInt32(condition,0));


    condition.CreateCondBr(conditionValue,bodyBlock,postBlock);

    /* body */
    llvm::Value * currentIndex = body.CreateShl(iValue, LlvmInt32(body,2), "currentIndex");
    llvm::Value * currentOffset = body.CreateAdd(array, currentIndex, "currentOffset");

    llvm::Value * previousIndex = body.CreateSub(iValue, LlvmInt32(body,1), "previousIndex");
    previousIndex = body.CreateShl(previousIndex, LlvmInt32(body,2), "previousIndex");
    llvm::Value * previousOffset = body.CreateAdd(array, previousIndex, "previousOffset");

    llvm::Value * previousValue = body.CreateLoad(previousOffset, "previousValue");

    llvm::Value * newValue = body.CreateShl(previousValue, LlvmInt32(body,1), "newValue");

    body.CreateStore(newValue, currentOffset, "update");

    // i = i + 1
    iValue = body.CreateAdd(iValue,LlvmInt32(condition,1));
    body.CreateStore(iValue,iLocation,"iStore");

    body.CreateBr(conditionBlock);

    /* post */

    llvm::Function::arg_iterator arguments = mainFunction->arg_begin();
    llvm::Value * requestedIndex = arguments;

    llvm::Value * retIndex = post.CreateShl(requestedIndex, LlvmInt32(post,2), "retIndex");
    llvm::Value * retOffset = post.CreateAdd(array, retIndex, "retOffset");

    llvm::Value * returnValue = post.CreateLoad(retOffset, "return");

    post.CreateRet(returnValue);

    // dump asm to stdout and execute
    llvm::outs() << *module.get();
    llvm::outs().flush();

    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(module)).create();

    // call main with the index
    int (*function)(int) = EE->getPointerToFunction(mainFunction);

    std::cout << "Result: " << function(0) << std::endl;

    EE->freeMachineCodeForFunction(mainFunction);

    delete EE;
    llvm::llvm_shutdown();
    return 0;
}

