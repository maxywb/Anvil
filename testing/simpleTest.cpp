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

#include <vector>
#include <iostream>

int main() {
  
    llvm::InitializeNativeTarget();

    llvm::LLVMContext context;
    // Create some module to put our function into it.
    std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("test", context);
    llvm::Module *M = module.get();

    // setup "main" function and top level basic block
    llvm::Function *mainFunction =
        llvm::cast<llvm::Function>(M->getOrInsertFunction("main", llvm::Type::getInt32Ty(context),
                                                          (llvm::Type *)0));
    
    /*
     * setup
     */

    llvm::BasicBlock *body = llvm::BasicBlock::Create(context, "body", mainFunction);
    llvm::IRBuilder<> bodyBuilder(body);

    llvm::Type * int32Type = llvm::Type::getInt32Ty(context);    

    llvm::Value * zero = bodyBuilder.getInt32(0);
    llvm::Value * one = bodyBuilder.getInt32(1);
    llvm::Value * ten = bodyBuilder.getInt32(10);

    // initialize x to 1
    llvm::AllocaInst * xLocation = bodyBuilder.CreateAlloca(int32Type,0,"xStore");
    bodyBuilder.CreateStore(one,xLocation,"xStore");
    llvm::Value * xValue = bodyBuilder.CreateLoad(xLocation,"xValue");

    // initialize i to 10
    llvm::AllocaInst * iLocation = bodyBuilder.CreateAlloca(int32Type,0,"iStore");
    bodyBuilder.CreateStore(ten,iLocation,"xStore");
    llvm::Value * iValue = bodyBuilder.CreateLoad(iLocation,"iValue");


    // add x and i
    llvm::Value * addValue = bodyBuilder.CreateAdd(xValue, iValue);

    // return result of x + 1
    bodyBuilder.CreateRet(addValue);


    
    // dump to stdout and execute

    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(module)).create();

    llvm::outs() << *M;
    llvm::outs().flush();

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
