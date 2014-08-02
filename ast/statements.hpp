#ifndef ANVIL_STATEMENT_HPP
#define ANVIL_STATEMENT_HPP

#include "ast.hpp"
#include "node.hpp"

#include <list>
#include <string>
#include <sstream>



namespace anvil{
  class Assignment : public Statement
  {
  private:
    std::string m_name;
    Expression * m_rhs;
  public:
    Assignment(std::string *name, Expression * rhs) 
      : m_name(*name),m_rhs(rhs) {}

    void setNext(Statement * next){
      m_next = next;
    }
    Statement *  next(){
      return m_next;
    }
    std::string print(){
      std::ostringstream strs;

      strs << m_name;
      strs << "=";
      strs << m_rhs->print();

      return strs.str();
    }

  };

  class ConditionalBranch : public Statement
  {
  private:
    Expression * m_condition;
    StatementList * m_body;
  public:
    
    std::string print(){
      return "if/else";
    }

  };

  class ConditionalStatement : public Statement
  {
  private:
    std::list<ConditionalBranch * > m_conditionBranches; // first is the if, others are elseif

  public:


  };


   class ForLoop : public Statement
   {
     
   };

  class WhileLoop : public Statement
   {
     
   };

}
#endif
