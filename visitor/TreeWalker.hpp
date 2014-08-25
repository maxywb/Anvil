#ifndef ANVIL_IR_TREEWALKER_HPP
#define ANVIL_IR_TREEWALKER_HPP

#include "ir/Term.hpp"
#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"
#include <iostream>

#include <memory>
#include <list>
#include <string>

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


  class TreeWalker
  {
  private:

    std::list<std::shared_ptr<Term> > m_terms;
    SymbolTable m_symbolTable;

    // get 1st operand of most recently added Term
    std::string getCurrentResult()
    {
      ASSERT(m_terms.size() > 0, "no terms to get result from");
      std::shared_ptr<Term> currentTerm = m_terms.back();
      return currentTerm->getFirstArgument();
      
    }
    
    void addTerm(std::string operation, std::list<std::string> operands)
    {
      m_terms.push_back(std::shared_ptr<Term>(new Term(operation,operands)));
    }

  public:
    
    
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


    void printTerms()
    {
      for(auto itr = m_terms.begin();
	itr != m_terms.end();
	itr++) {
	std::cout << (*itr)->print() << std::endl;
    }

    }

    
  };
}
#endif
