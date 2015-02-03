#ifndef ANVIL_IR_CODEEMITTER_HPP
#define ANVIL_IR_CODEEMITTER_HPP

#include "TreeWalker.hpp"

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
  class Integer;
  class Double;
  class Assignment;
  class BinaryOperator;
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

  struct Scope
  {
    llvm::BasicBlock * block;
    llvm::Function * function;
    Scope(llvm::BasicBlock * _block, llvm::Function * _function) : block(_block), function(_function) {};
  };


  class CodeEmitter : public TreeWalker
  {
  private:
    std::shared_ptr<SymbolTable> m_symbolTable;

    std::unique_ptr<llvm::Module> m_module;

    llvm::LLVMContext * m_context;
    llvm::Function * m_mainFunction;
    llvm::Function * m_malloc;
    llvm::Function * m_free;

    llvm::BasicBlock * m_currentBlock;
    std::unique_ptr<llvm::IRBuilder<> > m_currentBuilder;
    llvm::Function * m_currentFunction;
    
    std::stack<Scope> m_scope;

    NameGenerator & m_nameGenerator;

    void descendScope(llvm::BasicBlock * bb, llvm::Function * f);
    void ascendScope();

  public:
    CodeEmitter(std::shared_ptr<SymbolTable> symbolTable, NameGenerator & nameGen);
    ~CodeEmitter() override;
    
    void visit(Id * node) override;
    void visit(Assignment * node) override;
    void visit(BinaryOperator * node) override;
    void visit(Integer * node) override;
    void visit(Double * node) override;
    void visit(ConditionalBlock* node) override;
    void visit(ConditionalBranch* node) override;
    void visit(Expression * node) override;
    void visit(ForLoop * node) override;
    void visit(FunctionCall * node) override;
    void visit(FunctionDefinition * node) override;
    void visit(Statement * node) override;
    void visit(WhileLoop* node) override;
    void visit(ReturnStatement* node) override;

  };
}
#endif
