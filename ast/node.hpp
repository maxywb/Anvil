#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include "operators.hpp"
#include "expressions.hpp"

#include <string>
#include <list>

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

  class StatementList : public Node
  {
  protected:
      std::list<Statement *> m_statements;
  public:

      typedef std::list<Statement *>::iterator iterator;
      
      iterator begin(){
          return m_statements.begin();
      }

      iterator end(){
          return m_statements.end();
      }

      void push_front(Statement * stmt){
          m_statements.insert(m_statements.begin(),stmt);
      }

      std::string print(){
          return "statement list";
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
