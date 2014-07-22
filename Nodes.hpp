#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include <string>
#include <ostream>
#include <sstream>

namespace anvil{
  namespace operators{
      enum BinaryOperatorType {
	ADD,
	MULTIPLY
      };
  }

  class Node 
  {
  protected:
    virtual std::string toString(){
      return "node";
    }

  public:
    std::string print(){
      return toString();
    }
    Node(){}
    std::ostream& operator<<(std::ostream& os){
      os << toString();
    }  

  };

  class Expression : public Node
  {
  protected:
    Expression * m_parent;
    std::string toString(){
      return "expr";
    }

  public:
    Expression() {}
    void setParent(Expression * parent) {
      m_parent = parent;
    }

  };

  class Number : public Expression
  {
  private:
    double m_value;

  public:
    Number(double value) 
      :  m_value(value) {}

  protected:
    std::string toString(){
      std::ostringstream strs;
      strs << m_value;
      return strs.str();
    }

  };

  class BinaryOperator : public Expression
  {
  private:
    operators::BinaryOperatorType const m_operator;
    Expression * m_left;
    Expression * m_right;

  public:
    BinaryOperator(Expression * left, Expression * right,
		   operators::BinaryOperatorType op)
      : m_left(left), m_right(right), m_operator(op) {}

  protected:
    std::string toString(){
      std::ostringstream strs;
      strs << m_left;
      switch (m_operator){
      case operators::ADD:
	strs << "+";
	break;
      case operators::MULTIPLY:
	strs << "*";
	break;
      }
      strs << m_right;

      return strs.str();
    }

  };

}
#endif
