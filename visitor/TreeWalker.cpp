
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

  void TreeWalker::visit(Assignment * node)
  {
    std::cout << "assignment" << std::endl;
    
    node->getRHS()->visit(this);

    std::string endValue = m_symbolTable.addOrUpdateName(node->getName());

    addTerm("add",{endValue,"$zero",getCurrentResult()});

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

    case operators::LessThan:
      operation = "less_than";
      break;
    case operators::Comma:

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
      ASSERT(false,"tree walker: Unsupported binary operator type:" << expressionType);
      break;
    }


    
    std::shared_ptr<Term> currentTerm(new Term(operation,operands));

    m_terms.push_back(move(currentTerm));


  }

  void TreeWalker::visit(Id * node)
  {
    std::cout << "visit id" << std::endl;

    std::string myTemp = m_symbolTable.getName(node->getId());

    addTerm("add",{myTemp,"$zero",myTemp});
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
    std::cout << "visit number" << std::endl;
    addTerm("add",{m_symbolTable.getUniqueName(),"0x0",node->print()});

  }
  void TreeWalker::visit(ForLoop * node)
  {
    std::cout << "for loop" << std::endl;

    std::cout << "setup" << std::endl;

    Expression * initial = node->getInitial();
    Expression * condition = node->getCondition();
    Expression * counter = node->getCounter();
    
    std::string startLabel = m_symbolTable.getUniqueName();
    std::string endLabel = m_symbolTable.getUniqueName();
    
    std::cout << "initial" << std::endl;

    initial->visit(this);
    std::string initialResult = getCurrentResult();

    addTerm("label",{startLabel});

    std::cout << "condition" << std::endl;
    condition->visit(this);
    std::string conditionalResult = getCurrentResult();

    addTerm("jump_eq",{"$zero",conditionalResult,endLabel});
    
    std::cout << "counter" << std::endl;
    counter->visit(this);
    std::string counterResult = getCurrentResult();

    addTerm("add",{initialResult,"$zero",counterResult});

    node->getBody()->visit(this);

    addTerm("jump",{startLabel});

    addTerm("label",{endLabel});



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


