
#include "TreeWalker.hpp"

#include "ast/ast.hpp"
#include "ast/operators.hpp"
#include "ast/node.hpp"
#include "ast/expressions.hpp"
#include "ast/statements.hpp"
#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

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

#define LlvmInt32(builder, value) (builder->getInt32(value))

#define FunctionInputTerminator (static_cast<llvm::Type *>(0))

// wrappers for reduced visual clutter
  
#define getBuilder(currentBlock) (llvm::make_unique<llvm::IRBuilder<>>(currentBlock))
#define getInt32Type(context) (llvm::Type::getInt32Ty(*context))

namespace anvil{

  TreeWalker::~TreeWalker() 
  {
#ifdef DEBUG
    std::cout << "##### LLVM IR #####" << std::endl;
    llvm::outs() << *m_module.get();
    llvm::outs().flush();


#endif

    llvm::ExecutionEngine* EE = llvm::EngineBuilder(std::move(m_module)).create();
    int (*function)() = EE->getPointerToFunction(m_mainFunction);

#ifdef DEBUG
    std::cout << function() << std::endl;
#else
    function();
#endif

    EE->freeMachineCodeForFunction(m_mainFunction);
  }

  TreeWalker::TreeWalker() : m_symbolTable(new SymbolTable()) 
  {
    PRINT("treewalker" << std::endl;);



    llvm::InitializeNativeTarget();

    m_context = /*std::unique_ptr<llvm::LLVMContext>*/(new llvm::LLVMContext());

    // make a module to hold the function
    m_module = llvm::make_unique<llvm::Module>("module", *m_context);
    //
    // initialize "main" function

    m_mainFunction =
      (llvm::cast<llvm::Function>(m_module->getOrInsertFunction("main", 
								llvm::Type::getInt32Ty(*m_context), // output
								FunctionInputTerminator)));
    
    // initialize malloc 
    llvm::PointerType * mallocOutput = llvm::PointerType::get(llvm::Type::getVoidTy(*m_context),0);
    llvm::FunctionType * mallocPrototype = llvm::FunctionType::get(mallocOutput, {llvm::Type::getInt32Ty(*m_context)}, FunctionInputTerminator);
    m_malloc = llvm::cast<llvm::Function>(m_module->getOrInsertFunction("malloc",mallocPrototype));
      
    
    // initialize free
    llvm::PointerType * freeOutput = llvm::PointerType::get(llvm::Type::getVoidTy(*m_context),0);
    llvm::FunctionType * freePrototype = llvm::FunctionType::get(freeOutput, {llvm::Type::getInt32Ty(*m_context)}, FunctionInputTerminator);
    m_free = llvm::cast<llvm::Function>(m_module->getOrInsertFunction("free",freePrototype));

    // setup top level basic block
    m_currentBlock = llvm::BasicBlock::Create(*m_context, "mainBlock", m_mainFunction);
    m_currentBuilder = getBuilder(m_currentBlock);
    m_currentFunction = m_mainFunction;
  }


  void TreeWalker::visit(Expression * node)
  {


  }

  void TreeWalker::visit(Assignment * node)
  {
    PRINT("assignment" << std::endl);

    node->getRHS()->visit(this);

    llvm::AllocaInst * resultLocation;

    std::cout << node->getName() << std::endl;
    std::cout << m_symbolTable->hasName(node->getName()) << std::endl;

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

    
  void TreeWalker::visit(BinaryOperator * node)

  {
    Expression * right = node->getRight();
    Expression * left = node->getLeft();

    right->visit(this);
    llvm::Value * rightValue = right->getValue();

    ASSERT(rightValue, "right of binary operator doesn't have a llvm::Value");

    left->visit(this);
    llvm::Value * leftValue = left->getValue();

    ASSERT(leftValue, "left of binary operator doesn't have a llvm::Value");

    std::string resultName = m_symbolTable->getUniqueName();
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

  void TreeWalker::visit(Id * node)
  {
    PRINT("Id" << std::endl);

    std::string name = m_symbolTable->getName(node->getId());

    llvm::AllocaInst * location = m_symbolTable->getVariable(name);

    llvm::Value * loadedValue = m_currentBuilder->CreateLoad(location, name);

    node->setValue(loadedValue);

  }

  void TreeWalker::visit(Statement * node)
  {
    node->visit(this);
  }


  void TreeWalker::visit(StatementList * node)
  {
    for(StatementList::iterator itr = node->begin();
	itr != node->end();
	itr++) {
      visit(*itr);
    }
  }

  void TreeWalker::visit(Number * node)
  {
    PRINT("number" << std::endl);

    std::string resultName = m_symbolTable->getUniqueName();
    
    llvm::Value * currentValue = m_currentBuilder->CreateAdd(LlvmInt32(m_currentBuilder,node->getInt()), LlvmInt32(m_currentBuilder,0), resultName);
    
    node->setValue(currentValue);
  }

  void TreeWalker::visit(ForLoop * node)
  {


    Expression * setup = node->getInitial();
    Expression * condition = node->getCondition();
    Expression * increment = node->getCounter();
    StatementList * body = node->getBody();

    llvm::BasicBlock * setupBlock = llvm::BasicBlock::Create(*m_context, "setup", m_mainFunction);
    llvm::BasicBlock * conditionBlock = llvm::BasicBlock::Create(*m_context, "condition", m_mainFunction);
    llvm::BasicBlock * bodyBlock = llvm::BasicBlock::Create(*m_context, "body", m_mainFunction);
    llvm::BasicBlock * incrementBlock = llvm::BasicBlock::Create(*m_context, "increment", m_mainFunction);
    llvm::BasicBlock * postBlock = llvm::BasicBlock::Create(*m_context, "post", m_mainFunction);

    // setup
    m_currentBuilder->CreateBr(setupBlock);
    m_currentBuilder = getBuilder(setupBlock);
    m_currentBlock = setupBlock;
    setup->visit(this);

    m_currentBuilder->CreateBr(conditionBlock);

    // condition
    m_currentBuilder = getBuilder(conditionBlock);
    m_currentBlock = conditionBlock;
    condition->visit(this);
    
    m_currentBuilder->CreateCondBr(condition->getValue(), bodyBlock, postBlock);

    // body
    descendScope(bodyBlock, m_currentFunction);
    body->visit(this);
    ascendScope();
    m_currentBuilder->CreateBr(incrementBlock);

    // increment
    m_currentBuilder = getBuilder(incrementBlock);
    m_currentBlock = incrementBlock;
    increment->visit(this);

    m_currentBuilder->CreateBr(conditionBlock);

    // post
    m_currentBuilder = getBuilder(postBlock);
    m_currentBlock = postBlock;
  }

  void TreeWalker::visit(FunctionDefinition * node)
  {

    PRINT("function definition" << std::endl);
    
    llvm::Function * function =
      (llvm::cast<llvm::Function>(m_module->getOrInsertFunction("main", 
								llvm::Type::getInt32Ty(*m_context), // output
								FunctionInputTerminator)));

    m_symbolTable->addName(node->getName());
    
    


  }

  void TreeWalker::visit(WhileLoop* node)
  {

  }
  void TreeWalker::visit(ConditionalBranch* node)
  {

  }
  void TreeWalker::visit(ConditionalBlock* node)
  {

  }
  void TreeWalker::visit(ReturnStatement* node)
  {
    node->getExpression()->visit(this);

    llvm::Value * returnValue = node->getExpression()->getValue();
        
    m_currentBuilder->CreateRet(returnValue);

  }

  void TreeWalker::descendScope(llvm::BasicBlock * bb, llvm::Function * f)
  {

    m_scope.push(Scope(bb, f));

    m_currentBuilder = getBuilder(bb);
    m_currentBlock = bb;
    m_currentFunction = f;

  }

  void TreeWalker::ascendScope()
  {

    Scope parent = m_scope.top();
    m_scope.pop();
    
    m_currentBlock = parent.block;;
    m_currentFunction = parent.function;

    m_currentBuilder = getBuilder(m_currentBlock);

  }

  void TreeWalker::visit(FunctionCall * node)
  {
    PRINT("FunctionCall" << std::endl);
  }

}
