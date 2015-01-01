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

int main() {
  
    llvm::InitializeNativeTarget();

    llvm::LLVMContext context;
    // Create some module to put our function into it.
    std::unique_ptr<llvm::Module> module = llvm::make_unique<llvm::Module>("test", context);
    llvm::Module *M = module.get();

    // setup "main" function and top level basic block
    llvm::Function *mainFunction =
        llvm::cast<llvm::Function>(M->getOrInsertFunction("main", llvm::Type::getInt32Ty(context),
                                                          llvm::Type::getInt32Ty(context),
                                                          (llvm::Type *)0));
    
    llvm::BasicBlock *setup = llvm::BasicBlock::Create(context, "setup", mainFunction);
    llvm::IRBuilder<> setupBuilder(setup);

    llvm::BasicBlock *condition = llvm::BasicBlock::Create(context, "condition", mainFunction);
    llvm::IRBuilder<> conditionBuilder(condition);

    llvm::BasicBlock *body = llvm::BasicBlock::Create(context, "body", mainFunction);
    llvm::IRBuilder<> bodyBuilder(body);

    llvm::BasicBlock *post = llvm::BasicBlock::Create(context, "post", mainFunction);
    llvm::IRBuilder<> postBuilder(post);

    llvm::Type * int32Type = llvm::Type::getInt32Ty(context);    

    /*
     * setup
     */
    llvm::Value * zero = setupBuilder.getInt32(0);
    llvm::Value * one = setupBuilder.getInt32(1);
    llvm::Value * ten = conditionBuilder.getInt32(10);

    // initialize x
    llvm::AllocaInst * xLocation = setupBuilder.CreateAlloca(int32Type,0,"xvar");
    setupBuilder.CreateStore(zero,xLocation,"store 0 in x");
    llvm::Value * xValue = setupBuilder.CreateLoad(xLocation,"xValue");

    // initialize i
    llvm::AllocaInst * iLocation = setupBuilder.CreateAlloca(int32Type,0,"ivar");
    setupBuilder.CreateStore(zero,iLocation,"ivar");
    llvm::Value * iValue = setupBuilder.CreateLoad(iValue,"iValue");


    setupBuilder.CreateBr(condition);

    /*
     * condition
     */

    // initialize condition value
    llvm::Value * iTemp = conditionBuilder.CreateLoad(iValue,"ivar");
    llvm::Value * conditionValue = conditionBuilder.CreateSub(ten,iTemp);
    conditionValue = conditionBuilder.CreateICmpNE(conditionValue,zero);

    // increment i
    iTemp = conditionBuilder.CreateAdd(iTemp,one);
    setupBuilder.CreateStore(iTemp,iValue,"ivar");

    conditionBuilder.CreateCondBr(conditionValue,body,post);

    /*
     * body
     */

    // x = x + 1
    llvm::Value * xValueIncrement = bodyBuilder.CreateLoad(xValue,"xinc");
    xValueIncrement = bodyBuilder.CreateAdd(xValueIncrement,one);
    bodyBuilder.CreateStore(xValueIncrement,xValue,"xinc");

    bodyBuilder.CreateBr(condition);


    /*
     * post
     */
    llvm::Value * xReturn = postBuilder.CreateLoad(xValue,"x");
    postBuilder.CreateRet(xReturn);
    


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
