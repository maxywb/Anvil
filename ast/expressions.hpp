#ifndef ANVIL_EXPRESSION_HPP
#define ANVIL_EXPRESSION_HPP

#include "assert.hpp"
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
      : m_type(type),m_left(left), m_right(right) 
    {
#ifndef NDEBUG
      // assert on unspported operator type
      switch(type){
      case operators::Add:
      case operators::Multiply:
	break;
      case operators::Subtract:
      case operators::Divide:
      case operators::Modulo:
      case operators::GreaterThan:
      case operators::LessThan:
      case operators::GreaterThanOrEqual:
      case operators::LessThanOrEqual:
      case operators::Equal:
      case operators::NotEqual:
      case operators::Xor:
      case operators::And:
      case operators::Or:
      case operators::ShiftRight:
      case operators::ShiftLeft:
      case operators::Dot:
      case operators::Comma:
      default:	
	ASSERT(false,"Unsupported binary operator type");
	break;
      }
#endif
    }
    
    std::string print(){
      std::ostringstream strs;

      strs << m_left->print();
      switch(m_type){
      case operators::Add:
	strs << "+";
	break;
      case operators::Subtract:
	break;
      case operators::Multiply:
	strs << "*";
	break;
      case operators::Divide:
	break;
      case operators::Modulo:
	break;
      case operators::GreaterThan:
	break;
      case operators::LessThan:
	break;
      case operators::GreaterThanOrEqual:
	break;
      case operators::LessThanOrEqual:
	break;
      case operators::Equal:
	break;
      case operators::NotEqual:
	break;
      case operators::Xor:
	break;
      case operators::And:
	break;
      case operators::Or:
	break;
      case operators::ShiftRight:
	break;
      case operators::ShiftLeft:
	break;
      case operators::Dot:
	break;
      case operators::Comma:
	break;
      default:	
	ASSERT(false,"This shouldn't happen.");
	break;
      }
      
      strs << m_right->print();

      return strs.str();
    }

  };


}
#endif
