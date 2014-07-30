#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include "operators.hpp"
#include "Expression.hpp"

#include <string>

namespace anvil{

  class Node 
  {
  public:
    virtual std::string print(){
      return "node";
    }

    Node(){}
  };

  class Statement : public Node
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

  };

  class Expression : public Statement
  {
  protected:
    Expression * m_parent;

  public:
    std::string print(){
      return "expr";
    }
    Expression() {}
    void setParent(Expression * parent) {
      m_parent = parent;
    }
  };

}
#endif
