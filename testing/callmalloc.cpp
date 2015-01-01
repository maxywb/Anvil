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

/*
x = malloc(10*sizeof(int32));
for(i = 0; i < 10; i++) {

}
 */

#define LlvmInt32(builder, value) (builder.getInt32(value))

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
    llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(context, "EntryBlock", mainFunction);
    llvm::IRBuilder<> builder(basicBlock);

    // setup malloc function
    llvm::PointerType * outputType = llvm::PointerType::get(llvm::Type::getVoidTy(context),0);
    std::vector<llvm::Type *> params(1,llvm::Type::getInt32Ty(context));
    llvm::FunctionType *mallocType = llvm::FunctionType::get(outputType,params,false);
    llvm::Function *mallocFunction = llvm::cast<llvm::Function>(module->getOrInsertFunction("malloc",mallocType));


    llvm::Value * two = LlvmInt32(builder,2);
    llvm::Value * seven = builder.getInt32(7);
    llvm::Value * zero = bodyBuilder.getInt32(0);
    llvm::Value * one = bodyBuilder.getInt32(1);
    llvm::Value * ten = bodyBuilder.getInt32(10);
    
    llvm::CallInst * mallocResult = builder.CreateCall(mallocFunction, two);

    llvm::Value * ptrToInt32 = builder.CreatePointerCast(mallocResult,
							 llvm::PointerType::get(llvm::Type::getInt32Ty(context),0),"pointerToInt32");

    builder.CreateStore(seven,ptrToInt32);
    llvm::LoadInst * loadedSeven = builder.CreateLoad(ptrToInt32,"stored7");

    llvm::Value * nine = builder.CreateAdd(loadedSeven,two);
    

    builder.CreateRet(nine);
    
    /**

       {
       <evaluate conditon>
       br condition body loop_end
       }
       {
       label: body
       <body>
       br loop_start
       }
       {
       label: loop_end
       }
       


     */
    
    /*
{
EntryBlock:
  %1 = call void* @malloc(i32 7)
  store i32 7, void* %1
  %stored7 = load void* %1
  add void %stored7, i32 2
  ret void <badref>
}
     */



    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(module)).create();

    llvm::outs() << "We just constructed this LLVM module:\n\n" << *M;
    llvm::outs() << "\n\nRunning foo: ";
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
