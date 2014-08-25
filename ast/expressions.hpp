#ifndef ANVIL_EXPRESSION_HPP
#define ANVIL_EXPRESSION_HPP

#include "assert.hpp"
#include "ast.hpp"
#include "statements.hpp"
#include "visitor/TreeWalker.hpp"

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

    virtual void visit(TreeWalker * walker)
    {
      walker->visit(this);
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

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }

  };

  class Id : public Expression
  {
  private:
    std::string m_id;

  public:
    Id(std::string * id) 
      :  m_id(*id) {}
    
    std::string getId()
    {
      return m_id;
    }

    std::string print(){
      return getId();
    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }

  };


  class BinaryOperator : public Expression
  {
  protected:
    Expression * m_left;
    Expression * m_right;
    operators::BinaryOperatorType m_type;
  public:
    Expression * getLeft() 
    {
      return m_left;
    }
    Expression * getRight() 
    {
      return m_right;
    }
    BinaryOperator(operators::BinaryOperatorType type, Expression * left, Expression * right)
      : m_left(left), m_right(right),m_type(type)
    {
#ifndef NDEBUG
      // assert on unspported operator type
      switch(type){
      case operators::Add:
      case operators::Multiply:
      case operators::Comma:
      case operators::LessThan:
	break;
      case operators::Subtract:
      case operators::Divide:
      case operators::Modulo:
      case operators::GreaterThan:
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
	strs << "<";
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
	strs << ",";
	break;
      default:	
	ASSERT(false,"This shouldn't happen.");
	break;
      }
      
      strs << m_right->print();

      return strs.str();
    }

    operators::BinaryOperatorType getType()
    {
      return m_type;
    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }
  


  };

  class Assignment : public Expression
  {
  private:
    std::string m_name;
    Expression * m_rhs;
  public:
    Assignment(std::string *name, Expression * rhs) 
      : m_name(*name),m_rhs(rhs) {}

    std::string print(){
      std::ostringstream strs;

      strs << m_name;
      strs << "=";
      strs << m_rhs->print();

      return strs.str();
    }
    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }

    Expression * getRHS()
    {
      return m_rhs;
    }

    std::string getName()
    {
      return m_name;
    }

  };




}
#endif
