#ifndef ANVIL_IR_TREEWALKER_HPP
#define ANVIL_IR_TREEWALKER_HPP

#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

// becuase i can't get the forward declaration correct
#include "llvm/IR/IRBuilder.h"

#include <unordered_map>
#include <memory>
#include <list>
#include <string>
#include <stack>

namespace llvm {
  class LLVMContext;
  class Module;
  class Function;
  class BasicBlock;
}

namespace anvil
{


  class Assignment;
  class BinaryOperator;
  class Number;
  class ConditionalBlock;
  class ConditionalBranch;
  class Expression;
  class ForLoop;
  class FunctionCall;
  class FunctionDefinition;
  class Statement;
  class WhileLoop;
  class Id;
  class ReturnStatement;

  class Scope
  {
  public:
    llvm::BasicBlock * block;
    llvm::Function * function;
    Scope(llvm::BasicBlock * _block, llvm::Function * _function) : block(_block), function(_function) {};
  };


  class TreeWalker
  {
  private:
    std::unique_ptr<SymbolTable> m_symbolTable;

    std::unique_ptr<llvm::Module> m_module;

    llvm::LLVMContext * m_context;
    llvm::Function * m_mainFunction;
    llvm::Function * m_malloc;
    llvm::Function * m_free;

    llvm::BasicBlock * m_currentBlock;
    std::unique_ptr<llvm::IRBuilder<> > m_currentBuilder;
    llvm::Function * m_currentFunction;
    
    std::stack<Scope> m_scope;

    void descendScope(llvm::BasicBlock * bb, llvm::Function * f);
    void ascendScope();

  public:
    TreeWalker();
    ~TreeWalker();
    
    void visit(Id * node);
    void visit(Assignment * node);
    void visit(BinaryOperator * node);
    void visit(Number * node);
    void visit(ConditionalBlock* node);
    void visit(ConditionalBranch* node);
    void visit(Expression * node);
    void visit(ForLoop * node);
    void visit(FunctionCall * node);
    void visit(FunctionDefinition * node);
    void visit(Statement * node);
    void visit(WhileLoop* node);
    void visit(ReturnStatement* node);

  };
}
#endif
