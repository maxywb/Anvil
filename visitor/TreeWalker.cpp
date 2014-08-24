
#include "TreeWalker.hpp"

#include "ast/ast.hpp"
#include "ast/operators.hpp"
#include "ast/node.hpp"
#include "ast/expressions.hpp"
#include "ast/statements.hpp"

#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

#include <iostream>
#include <memory>
#include <list>

namespace anvil{


  void TreeWalker::visit(Expression * node)
  {
    std::cout << "visiting expression: " << node->print() << std::endl;

  }

  void TreeWalker::visit(Statement * node)
  {
    std::cout << "visiting statement: " << node->print() << std::endl;
    node->visit(this);
  }

  void TreeWalker::visit(BinaryOperator * node)
  {
    node->getRight()->visit(this);
    std::string rightResult = getCurrentResult();
    std::cout << "rightResult " << rightResult << std::endl;

    node->getLeft()->visit(this);
    std::string leftResult = getCurrentResult();
    std::cout << "leftResult " << leftResult << std::endl;
    
    operators::BinaryOperatorType expressionType = node->getType();

    std::string operation;
    std::list<std::string> operands = {m_symbolTable.getUniqueName(),leftResult, rightResult};
    
    

    switch(expressionType){
    case operators::Add:
      std::cout << "visit add" << std::endl;
    
      operation = "add";

      break;
    case operators::Multiply:
      std::cout << "visit mult" << std::endl;
      operation = "multiply";
      break;

    case operators::Comma:
    case operators::LessThan:
    case operators::Subtract:
    case operators::Divide:
    case operators::Modulo:
    case operators::GreaterThan:
    case operators::GreaterThanOrEqual:
    case operators::LessThanOrEqual:
    case operators::Equal:
    case operators::NotEqual:
    case operators::Xor:
    case operators::And:
    case operators::Or:
    case operators::ShiftRight:
    case operators::ShiftLeft:
    case operators::Dot:
    default:	
      ASSERT(false,"tree walker: Unsupported binary operator type");
      break;
    }


    
    std::shared_ptr<Term> currentTerm(new Term(operation,operands));

    m_terms.push_back(move(currentTerm));


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
    std::cout << "visit number" << std::endl;
     m_terms.push_back(
		     std::shared_ptr<Term>(
					   new Term(
						    "add",{m_symbolTable.getUniqueName(),"0x0",node->print()}
 						    )
					   )
		     );

  }
  void TreeWalker::visit(ForLoop * node)
  {

  }
  void TreeWalker::visit(FunctionDefinition * node)
  {

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

}


