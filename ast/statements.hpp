#ifndef ANVIL_STATEMENT_HPP
#define ANVIL_STATEMENT_HPP

#include "ast.hpp"
#include "node.hpp"

#include <list>
#include <string>
#include <sstream>
#include <ostream>


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
      Expression * m_condition; // this is NULL when it's an else
    StatementList * m_body;
  public:

      ConditionalBranch(Expression * condition, StatementList * body) 
          : m_condition(condition), m_body(body) {}
    
    std::string print(){
      return "if/elif/else\n";
    }

  };

  class ConditionalBlock : public Statement
  {
  private:
      ConditionalBranch * m_ifBranch;
      std::list<ConditionalBranch * > * m_elifBranches;
      ConditionalBranch * m_elseBranch;

      ConditionalBlock() {}

  public:
      ConditionalBlock(ConditionalBranch * ifBranch, std::list<ConditionalBranch * > * elifBranches,ConditionalBranch * elseBranch) 
          : m_ifBranch(ifBranch), m_elifBranches(elifBranches), m_elseBranch(elseBranch) {}


    std::string print(){
      std::ostringstream strs;


      strs << m_ifBranch->print();

      for(std::list<ConditionalBranch *>::iterator itr = m_elifBranches->begin();
	  itr != m_elifBranches->end();
	  itr++){
          strs << (*itr)->print();
      }

      if(m_elseBranch){
          strs << m_elseBranch->print();
      }

      strs << std::endl;

    return strs.str();

    }

  };


   class ForLoop : public Statement
   {
     
   };

  class WhileLoop : public Statement
   {
     
   };

}
#endif