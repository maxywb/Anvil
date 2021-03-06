
#include "CodeEmitter.hpp"

#include "ast/ast.hpp"
#include "ast/operators.hpp"
#include "ast/node.hpp"
#include "ast/expressions.hpp"
#include "ast/statements.hpp"
#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/MCJIT.h"
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

#define LlvmInt32(builder, value) (builder->getInt32(value))

#define FunctionInputTerminator (static_cast<llvm::Type *>(0))

// wrappers for reduced visual clutter
  
#define getBuilder(currentBlock) (llvm::make_unique<llvm::IRBuilder<>>(currentBlock))
#define getInt32Type(context) (llvm::Type::getInt32Ty(*context))

namespace anvil{

  CodeEmitter::CodeEmitter(std::shared_ptr<SymbolTable> symbolTable, NameGenerator & nameGen) :
    m_symbolTable(symbolTable),
    m_nameGenerator(nameGen)
  {
    PRINT("code emitter" << std::endl;);

    llvm::InitializeNativeTarget();

    m_context = new llvm::LLVMContext();

    // make a module to hold the function
    m_module = llvm::make_unique<llvm::Module>("module", *m_context);
    //
    // initialize "main" function

    m_mainFunction =
      (llvm::cast<llvm::Function>(m_module->getOrInsertFunction("main", 
								llvm::Type::getInt32Ty(*m_context), // output
								FunctionInputTerminator)));
    
    // initialize malloc 
#if 0
    llvm::PointerType * mallocOutput = llvm::PointerType::get(llvm::Type::getVoidTy(*m_context),0);
    llvm::FunctionType * mallocPrototype = llvm::FunctionType::get(mallocOutput, {llvm::Type::getInt32Ty(*m_context)}, FunctionInputTerminator);
    m_malloc = llvm::cast<llvm::Function>(m_module->getOrInsertFunction("malloc",mallocPrototype));
      
    
    // initialize free
    llvm::PointerType * freeOutput = llvm::PointerType::get(llvm::Type::getVoidTy(*m_context),0);
    llvm::FunctionType * freePrototype = llvm::FunctionType::get(freeOutput, {llvm::Type::getInt32Ty(*m_context)}, FunctionInputTerminator);
    m_free = llvm::cast<llvm::Function>(m_module->getOrInsertFunction("free",freePrototype));
#endif
    // setup top level basic block
    m_currentBlock = llvm::BasicBlock::Create(*m_context, "mainBlock", m_mainFunction);
    m_currentBuilder = getBuilder(m_currentBlock);
    m_currentFunction = m_mainFunction;
  }

  CodeEmitter::~CodeEmitter() 
  {
#ifdef DEBUG
    std::cout << "##### LLVM IR #####" << std::endl;
    llvm::outs() << *m_module.get();
    llvm::outs().flush();
    std::cout << "# build function" << std::endl;
#endif
    
    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(m_module)).create();
                                
    EE->finalizeObject();

    int (*function)() = EE->getPointerToFunction(m_mainFunction);

#ifdef DEBUG
    std::cout << "#" << std::endl;
    std::cout << function() << std::endl;
    std::cout << "#" << std::endl;
#else
    function();
#endif

    //EE->freeMachineCodeForFunction(m_mainFunction);
  }

  void CodeEmitter::visit(Expression * node)
  {
    node->visit(this);
  }

  void CodeEmitter::visit(Assignment * node)
  {
    PRINT("assignment" << std::endl);

    node->getRHS()->visit(this);

    llvm::AllocaInst * resultLocation;

    if (m_symbolTable->hasName(node->getName())) {
      // already exists
      std::string resultName = m_symbolTable->getName(node->getName());
      resultLocation = m_symbolTable->getVariable(resultName);
    } else {
      // new allocation
      std::string resultName = m_symbolTable->addName(node->getName());
      resultLocation = m_currentBuilder->CreateAlloca(getInt32Type(m_context),0,resultName);

      m_symbolTable->storeVariable(resultName, resultLocation);
    }


    llvm::Value * resultValue = node->getRHS()->getValue();

    m_currentBuilder->CreateStore(resultValue, resultLocation);

    node->setValue(resultValue);
  }

    
  void CodeEmitter::visit(BinaryOperator * node)

  {
    Expression * right = node->getRight();
    Expression * left = node->getLeft();

    right->visit(this);
    llvm::Value * rightValue = right->getValue();

    ASSERT(rightValue, "right of binary operator doesn't have a llvm::Value");

    left->visit(this);
    llvm::Value * leftValue = left->getValue();

    ASSERT(leftValue, "left of binary operator doesn't have a llvm::Value");

    std::string resultName = m_nameGenerator.getUniqueName();
    llvm::Value * resultValue = NULL;

    operators::BinaryOperatorType expressionType = node->getType();


    switch(expressionType) {

    case operators::Add:

      
      resultValue = m_currentBuilder->CreateAdd(leftValue, rightValue, resultName);

      break;

    case operators::LessThan:

      resultValue = m_currentBuilder->CreateICmpSLT(leftValue, rightValue, resultName);

      break;

    case operators::Subtract:

      break;



    case operators::And:
    case operators::Comma:
    case operators::Divide:
    case operators::Dot:
    case operators::Equal:
    case operators::GreaterThan:
    case operators::GreaterThanOrEqual:
    case operators::LessThanOrEqual:
    case operators::Modulo:
    case operators::Multiply:
    case operators::NotEqual:
    case operators::Or:
    case operators::ShiftLeft:
    case operators::ShiftRight:
    case operators::Xor:
    default:	
      ASSERT(false,"tree walker: Unsupported binary operator type:" << expressionType);
      break;
    }

    ASSERT(resultValue, "no result of binary operator");

    node->setValue(resultValue);

  }

  void CodeEmitter::visit(Id * node)
  {
    PRINT("Id" << std::endl);

    std::string name = m_symbolTable->getName(node->getId());

    llvm::AllocaInst * location = m_symbolTable->getVariable(name);

    llvm::Value * loadedValue = m_currentBuilder->CreateLoad(location, name);

    node->setValue(loadedValue);

  }

  void CodeEmitter::visit(Statement * node)
  {
    node->visit(this);
  }

  void CodeEmitter::visit(Double * node)
  {
    ASSERT(false, "Double not implemented");
  }

  void CodeEmitter::visit(Integer * node)
  {
    PRINT("Number" << std::endl);

    std::string resultName = m_nameGenerator.getUniqueName();

    llvm::Value * currentValue = m_currentBuilder->CreateAdd(LlvmInt32(m_currentBuilder,node->getValue()), LlvmInt32(m_currentBuilder,0), resultName);
    
    node->setValue(currentValue);
  }

  void CodeEmitter::visit(ForLoop * node)
  {


    Expression * setup = node->getInitial();
    Expression * condition = node->getCondition();
    Expression * increment = node->getCounter();
    std::list<Statement *> body = node->getBody();

    llvm::BasicBlock * setupBlock = llvm::BasicBlock::Create(*m_context, "setup", m_currentFunction);
    llvm::BasicBlock * conditionBlock = llvm::BasicBlock::Create(*m_context, "condition", m_currentFunction);
    llvm::BasicBlock * bodyBlock = llvm::BasicBlock::Create(*m_context, "body", m_currentFunction);
    llvm::BasicBlock * incrementBlock = llvm::BasicBlock::Create(*m_context, "increment", m_currentFunction);
    llvm::BasicBlock * postBlock = llvm::BasicBlock::Create(*m_context, "post", m_currentFunction);

    // setup
    m_currentBuilder->CreateBr(setupBlock);

    m_currentBuilder = getBuilder(setupBlock);
    m_currentBlock = setupBlock;

    setup->visit(this);

    m_currentBuilder->CreateBr(conditionBlock);

    // condition
    m_currentBlock = conditionBlock;
    m_currentBuilder = getBuilder(conditionBlock);

    condition->visit(this);
    
    m_currentBuilder->CreateCondBr(condition->getValue(), bodyBlock, postBlock);

    // body
    descendScope(bodyBlock, m_currentFunction);
    m_currentBuilder = getBuilder(bodyBlock);

    for (auto stmt : body) {
      stmt->visit(this);
    }

    ascendScope();

    m_currentBuilder->CreateBr(incrementBlock);

    // increment
    m_currentBuilder = getBuilder(incrementBlock);
    m_currentBlock = incrementBlock;
    increment->visit(this);

    m_currentBuilder->CreateBr(conditionBlock);

    // post
    m_currentBlock = postBlock;
    m_currentBuilder = getBuilder(postBlock);
  }

  void CodeEmitter::visit(FunctionDefinition * node)
  {

    PRINT("function definition" << std::endl);

    std::list<Id *> parameters = node->getParameters();

    llvm::Type * outputType = llvm::Type::getInt32Ty(*m_context);
    std::vector<llvm::Type *> llvmParameters(parameters.size(),getInt32Type(m_context));
    llvm::FunctionType * functionType = llvm::FunctionType::get(outputType,llvmParameters,false);
    llvm::Function * function = llvm::cast<llvm::Function>(m_module->getOrInsertFunction(node->getName(),functionType));

    m_symbolTable->storeFunctionDefinition(node->getName(), function);

    // m_symbolTable->descendScope();
    //     
    m_symbolTable = m_symbolTable->makeChild();

    // setup top level basic block
    llvm::BasicBlock * captureBlock = llvm::BasicBlock::Create(*m_context, "load_captures", function);
    llvm::BasicBlock * argumentsBlock = llvm::BasicBlock::Create(*m_context, "load_function_arguments", function);
    llvm::BasicBlock * firstBlock = llvm::BasicBlock::Create(*m_context, "first", function);

    //
    // TODO: abstract state saving to be more generic
    //       maybe...
    //       depending on how many there are
    //       becuase it's almost certainly faster this way
    //

    // save state
    // alocate/assign the new stuff here to be std::swap'd later
    llvm::BasicBlock * blockHolder = nullptr;
    std::unique_ptr<llvm::IRBuilder<>> builderHolder = nullptr;
    llvm::Function * functionHolder = nullptr;

    // new state
    std::swap(blockHolder, m_currentBlock);
    std::swap(builderHolder, m_currentBuilder);
    std::swap(functionHolder, m_currentFunction);

    m_currentBlock = argumentsBlock;
    m_currentBuilder = getBuilder(m_currentBlock);
    m_currentFunction = function;

    llvm::Function::arg_iterator arguments = function->arg_begin();
    
    for (Id * p : parameters) {
      std::string argumentName = m_symbolTable->addName(p->getId());
      llvm::Value * argumentValue = arguments;
      ++arguments;

      // translate the input argument into a regular variable
      llvm::AllocaInst * argumentVariable = m_currentBuilder->CreateAlloca(getInt32Type(m_context),0,argumentName);
      m_currentBuilder->CreateStore(argumentValue, argumentVariable);

      m_symbolTable->storeVariable(argumentName, argumentVariable);
    }

    m_currentBuilder->CreateBr(firstBlock);
    m_currentBlock = firstBlock;
    m_currentBuilder = getBuilder(m_currentBlock);

    for (auto stmt : node->getBody()) {
      stmt->visit(this);
    }

    //m_symbolTable->ascendScope();
    m_symbolTable = m_symbolTable->getParent();


    // load closures
    m_currentBlock = captureBlock;
    m_currentBuilder = getBuilder(m_currentBlock);
    m_currentBuilder->CreateBr(argumentsBlock);

    // TODO:
    // - get capture list from symbol table
    // make struct to contain them

    // restore old state
    std::swap(blockHolder, m_currentBlock);
    std::swap(builderHolder, m_currentBuilder);
    std::swap(functionHolder, m_currentFunction);




  }

  void CodeEmitter::visit(WhileLoop* node)
  {
    ASSERT(false, "WhileLoop not implemented");
  }

  void CodeEmitter::visit(ConditionalBranch* node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CodeEmitter::visit(ConditionalBlock* node)
  {
    ASSERT(false, "ConditionalBlock not implemented");
  }

  void CodeEmitter::visit(ReturnStatement* node)
  {
    PRINT("ReturnStatement" << std::endl);

    node->getExpression()->visit(this);

    llvm::Value * returnValue = node->getExpression()->getValue();
    
    m_currentBuilder->CreateRet(returnValue);

  }

  void CodeEmitter::descendScope(llvm::BasicBlock * bb, llvm::Function * f)
  {

    m_scope.push(Scope(bb, f));

    m_currentBuilder = getBuilder(bb);
    m_currentBlock = bb;
    m_currentFunction = f;

  }

  void CodeEmitter::ascendScope()
  {

    Scope parent = m_scope.top();
    m_scope.pop();
    
    m_currentBlock = parent.block;;
    m_currentFunction = parent.function;

    m_currentBuilder = getBuilder(m_currentBlock);

  }

  void CodeEmitter::visit(FunctionCall * node)
  {
    PRINT("FunctionCall" << std::endl);
    
    std::vector<llvm::Value *> functionArguments;

    for (Expression * arg : node->getArguments()) {
      arg->visit(this);
      functionArguments.push_back(arg->getValue());
    }
    
    //    CreateInvoke (Value *Callee, BasicBlock *NormalDest, BasicBlock *UnwindDest, ArrayRef< Value * > Args, const Twine &Name="")
    
    llvm::Function * functionToCall = m_symbolTable->getFunctionDefinition(node->getName());
    llvm::Value * functionCallResult = m_currentBuilder->CreateCall(functionToCall, functionArguments);
    
    node->setValue(functionCallResult);
    
  }

}
