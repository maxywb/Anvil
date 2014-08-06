#ifndef ANVIL_EXPRESSION_HPP
#define ANVIL_EXPRESSION_HPP

#include "ast.hpp"
#include "statements.hpp"

#include <string>
#include <sstream>
#include <ostream>

namespace anvil{

  class FunctionCall : public Expression
  {
  private:
    std::string m_name;
    Expression * m_arguments;

  public:
    FunctionCall(std::string name, Expression * args) 
      : m_name(name), m_arguments(args) {}
    std::string print(){
      std::ostringstream strs;
      strs << m_name;
      strs << "(";
      if(m_arguments){
	strs << m_arguments->print();
      }
      strs << ")";
      return strs.str();
    }

  };

  class Number : public Expression
  {
  private:
    double m_value;

  public:
    Number(double value) 
      :  m_value(value) {}
    
    std::string print(){
      std::ostringstream strs;
      strs << m_value;
      return strs.str();
    }


  };

  class BinaryOperator : public Expression
  {
  protected:
    Expression * m_left;
    Expression * m_right;
    operators::BinaryOperatorType m_type;
  public:
    BinaryOperator(operators::BinaryOperatorType type, Expression * left, Expression * right)
      : m_type(type),m_left(left), m_right(right) {}

  };


}
#endif
