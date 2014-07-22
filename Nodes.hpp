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
    size_t m_id;
    virtual std::string toString();

  public:
    explicit Node(size_t id) 
      : m_id(id) {}

    std::ostream& operator<<(std::ostream& os){
      os << toString();
    }  

  };

  class Expression : public Node
  {
  protected:
    Expression * m_parent;

  public:
    Expression(size_t id, Expression * parent) 
      : Node(id), m_parent(parent) {}

  };

  class Number : public Expression
  {
  private:
    double m_value;

  public:
    Number(size_t id, Expression * parent, double value) 
      : Expression(id,parent), m_value(value) {}

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
    BinaryOperator(size_t id, Expression * parent, Expression * left, Expression * right,
		   operators::BinaryOperatorType op)
      : Expression(id,parent), m_left(left), m_right(right), m_operator(op) {}

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
