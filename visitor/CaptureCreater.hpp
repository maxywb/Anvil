#ifndef ANVIL_IR_CAPTURECREATOR_HPP
#define ANVIL_IR_CAPTURECREATOR_HPP

#include "TreeWalker.hpp"

#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

#include <unordered_map>
#include <memory>
#include <list>
#include <string>
#include <stack>

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

  class CaptureCreater : public TreeWalker
  {
  private:
    std::shared_ptr<SymbolTable> m_symbolTable;
  public:
    CaptureCreater(std::shared_ptr<SymbolTable> symbolTable);
    ~CaptureCreater() override;
    
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
