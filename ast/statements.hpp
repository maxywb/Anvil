#ifndef ANVIL_STATEMENT_HPP
#define ANVIL_STATEMENT_HPP

#include "Node.hpp"

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


}
#endif
