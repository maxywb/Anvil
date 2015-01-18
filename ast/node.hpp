#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include "operators.hpp"
#include "expressions.hpp"
#include "visitor/TreeWalker.hpp"

#include "llvm/ExecutionEngine/GenericValue.h"

#include <string>
#include <list>
#include <sstream>

namespace anvil{


  class Statement 
  {
  protected:
    Statement * m_next;
  public:
    void setNext(Statement * next){
      m_next = next;
    }
    Statement *  next(){
      return m_next;
    }

    virtual void visit(TreeWalker * walker) =0;

    virtual std::string print()=0;





  };

  class Expression : public Statement
  {
  protected:
    Expression * m_parent;

    llvm::Value * m_value;
    
  public:
    std::string print(){
      return "expr";
    }

    void setValue(llvm::Value * newValue)
    {
      ASSERT(newValue, "setting NULL value");
      m_value = newValue;
    }

    llvm::Value * getValue()
    {
      ASSERT(m_value, "requested NULL value");
      return m_value;
    }

    void setParent(Expression * parent) {
      m_parent = parent;
    }

    virtual void visit(TreeWalker * walker) =0;


  };

}
#endif
