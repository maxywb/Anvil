
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
  {  }

  CaptureCreater::CaptureCreater(std::shared_ptr<SymbolTable> symbolTable) : m_symbolTable(symbolTable) 
  {  }

  void CaptureCreater::visit(Expression * node)
  {  }

  void CaptureCreater::visit(Assignment * node)
  {  }

  void CaptureCreater::visit(BinaryOperator * node)

  {  }

  void CaptureCreater::visit(Id * node)
  {  }

  void CaptureCreater::visit(Statement * node)
  {  }

  void CaptureCreater::visit(Double * node)
  {  }

  void CaptureCreater::visit(Integer * node)
  {  }

  void CaptureCreater::visit(ForLoop * node)
  {  }

  void CaptureCreater::visit(FunctionDefinition * node)
  {  }

  void CaptureCreater::visit(WhileLoop* node)
  {  }

  void CaptureCreater::visit(ConditionalBranch* node)
  {  }

  void CaptureCreater::visit(ConditionalBlock* node)
  {  }

  void CaptureCreater::visit(ReturnStatement* node)
  {  }

  void CaptureCreater::visit(FunctionCall * node)
  {  }

}
