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
  class FunctionDefinition;
  class StatementList;
  class Statement;
  class WhileLoop;
  class Id;
  class ReturnStatement;


  class TreeWalker
  {
  private:
    SymbolTable m_symbolTable;

    std::unique_ptr<llvm::Module> m_module;

    llvm::LLVMContext * m_context;
    llvm::Function * m_mainFunction;
    llvm::Function * m_malloc;
    llvm::Function * m_free;

    llvm::BasicBlock * m_currentBlock;
    std::unique_ptr<llvm::IRBuilder<> > m_currentBuilder;

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
    void visit(FunctionDefinition * node);
    void visit(Statement * node);
    void visit(StatementList * node);
    void visit(WhileLoop* node);
    void visit(ReturnStatement* node);

  };
}
#endif
