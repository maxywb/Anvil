#ifndef ANVIL_IR_TREEWALKER_HPP
#define ANVIL_IR_TREEWALKER_HPP

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

  class TreeWalker
  {

  public:

    virtual ~TreeWalker() {}
    
    virtual void visit(Id * node) = 0;
    virtual void visit(Assignment * node) = 0;
    virtual void visit(BinaryOperator * node) = 0;
    virtual void visit(Integer * node) = 0;
    virtual void visit(Double * node) = 0;
    virtual void visit(ConditionalBlock* node) = 0;
    virtual void visit(ConditionalBranch* node) = 0;
    virtual void visit(Expression * node) = 0;
    virtual void visit(ForLoop * node) = 0;
    virtual void visit(FunctionCall * node) = 0;
    virtual void visit(FunctionDefinition * node) = 0;
    virtual void visit(Statement * node) = 0;
    virtual void visit(WhileLoop* node) = 0;
    virtual void visit(ReturnStatement* node) = 0;
    
  };
}
#endif
