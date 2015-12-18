
#include "CaptureCreater.hpp"

#include "ast/ast.hpp"
#include "ast/operators.hpp"
#include "ast/node.hpp"
#include "ast/expressions.hpp"
#include "ast/statements.hpp"
#include "symbolTable/SymbolTable.hpp"

#include "assert.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace anvil{

  CaptureCreater::~CaptureCreater() 
  {  
  }

  CaptureCreater::CaptureCreater(std::shared_ptr<SymbolTable> symbolTable) : 
    m_symbolTable(symbolTable) 
  {  
  }

  void CaptureCreater::visit(Assignment * node)
  {
    PRINT("assignment" << std::endl);

    node->getRHS()->visit(this);


    if (m_symbolTable->hasName(node->getName())) {
      // already exists
      return;
    } else {
      // new allocation
      std::string resultName = m_symbolTable->addName(node->getName());
      resultLocation = m_currentBuilder->CreateAlloca(getInt32Type(m_context),0,resultName);

      m_symbolTable->storeVariable(resultName, resultLocation);
    }

  }

  void CaptureCreater::visit(BinaryOperator * node)
  {
    Expression * right = node->getRight();
    Expression * left = node->getLeft();

    right->visit(this);
    left->visit(this);
  }

  void CaptureCreater::visit(Id * node)
  {
    // TODO:
    // look for assignment or use of variable in enclosing scopes
    if (m_symbolTable->getParent()->hasName(node->getId())) {
      m_symbolTable->addCapture(node->getId());
    }
  }

  void CaptureCreater::visit(ForLoop * node)
  {
    Expression * setup = node->getInitial();
    Expression * condition = node->getCondition();
    Expression * increment = node->getCounter();
    std::list<Statement *> body = node->getBody();

    setup->visit(this);

    condition->visit(this);

    for (auto stmt : body) {
      stmt->visit(this);
    }

    increment->visit(this);
  }

  void CaptureCreater::visit(FunctionDefinition * node)
  {
    for (auto stmt : node->getBody()) {
      stmt->visit(this);
    }
  }

  void CaptureCreater::visit(WhileLoop* node)
  {
    ASSERT(false, "WhileLoop not implemented");
  }

  void CaptureCreater::visit(ConditionalBranch* node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CaptureCreater::visit(Statement * node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CaptureCreater::visit(Expression* node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CaptureCreater::visit(Double * node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CaptureCreater::visit(Integer * node)
  {
    ASSERT(false, "ConditionalBranch not implemented");
  }

  void CaptureCreater::visit(ConditionalBlock* node)
  {
    ASSERT(false, "ConditionalBlock not implemented");
  }

  void CaptureCreater::visit(ReturnStatement* node)
  {
    node->getExpression()->visit(this);
  }

  void CaptureCreater::visit(FunctionCall * node)
  {
    std::vector<llvm::Value *> functionArguments;

    for (Expression * arg : node->getArguments()) {
      arg->visit(this);
    }
  }

}
