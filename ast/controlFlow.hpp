#ifndef ANVIL_CONTROL_FLOW_HPP
#define ANVIL_CONTROL_FLOW_HPP

#include "ast.hpp"
#include "statements.hpp"
#include "expressions.hpp"

#include <list>
#include <string>
#include <sstream>
#include <ostream>

namespace anvil{
  
  

  class ForLoop : public Statement
  {
  private:
    Expression * m_initial;
    Expression * m_condition;
    Expression * m_counter;

    std::list<Statement *> m_body;

  public:
    ForLoop(Expression * initial, Expression * condition, Expression * counter, 
	    std::list<Statement *> * body)
      : m_initial(initial), m_condition(condition), m_counter(counter), m_body(*body) {}

    std::string print(){
      
      return "for loop";

    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }


    Expression * getInitial()
    {
      return m_initial;
    }

    Expression * getCondition()
    {
      return m_condition;
    }

    Expression * getCounter()
    {
      return m_counter;
    }

    std::list<Statement *> getBody()
    {
      return m_body;
    }

  };

  class WhileLoop : public Statement
  {
    Expression * m_condition;

    std::list<Statement *> m_body;

  public:
    WhileLoop(Expression * condition, std::list<Statement *> * body)
      : m_condition(condition), m_body(*body) {}

    std::string print(){
      
      return "while loop";
      
    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }

     
  };



  
}
#endif
