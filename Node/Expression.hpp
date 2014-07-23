#ifndef ANVIL_EXPRESSION_HPP
#define ANVIL_EXPRESSION_HPP

#include "Node.hpp"

#include <string>
#include <sstream>
#include <ostream>

namespace anvil{
  class Expression : public Node
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

  public:
    BinaryOperator(Expression * left, Expression * right)
      : m_left(left), m_right(right) {}

  };

  class Comma : public BinaryOperator
  {
  public:
    Comma(Expression * left, Expression * right)		   
      : BinaryOperator(left,right) {}

    std::string print(){
      std::ostringstream strs;
      strs << "[";
      strs << m_left->print();
      strs << ",";
      strs << m_right->print();
      strs << "]";
      return strs.str();
    }

  };

  class Addition : public BinaryOperator
  {
  public:
    Addition(Expression * left, Expression * right)		   
      : BinaryOperator(left,right) {}

    std::string print(){
      std::ostringstream strs;
      strs << "(";
      strs << "+";
      strs << " ";
      strs << m_left->print();
      strs << ",";
      strs << m_right->print();
      strs << ")";
      return strs.str();
    }

  };

  class Multiplication : public BinaryOperator
  {
  public:
    Multiplication(Expression * left, Expression * right)		   
      : BinaryOperator(left,right) {}

    std::string print(){
      std::ostringstream strs;
      strs << "(";
      strs << "*";
      strs << " ";
      strs << m_left->print();
      strs << ",";
      strs << m_right->print();
      strs << ")";
      return strs.str();
    }

  };



}
#endif
