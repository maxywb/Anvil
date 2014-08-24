#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include "operators.hpp"
#include "expressions.hpp"
#include "visitor/TreeWalker.hpp"

#include <string>
#include <list>
#include <sstream>

namespace anvil{


  class Statement 
  {
  protected:
    Statement * m_next;
  public:
    void setNext(Statement * next){
      m_next = next;
    }
    Statement *  next(){
      return m_next;
    }

    virtual void visit(TreeWalker * walker) =0;

    virtual std::string print()=0;





  };

  class StatementList
  {
  protected:
    std::list<Statement *> m_statements;
  public:

    typedef std::list<Statement *>::iterator iterator;
      
    iterator begin(){
      return m_statements.begin();
    }

    iterator end(){
      return m_statements.end();
    }

    void push_front(Statement * stmt){
      m_statements.insert(m_statements.begin(),stmt);
    }

    std::string print(){
      std::ostringstream strs;
      strs << "sl: ";
      for(std::list<Statement *>::iterator itr = m_statements.begin();
	  itr != m_statements.end();
	  itr++){
	strs << (*itr)->print();
	strs << std::endl;
      }
	
      return strs.str();
    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }


  };


  class Expression : public Statement
  {
  protected:
    Expression * m_parent;

  public:
    std::string print(){
      return "expr";
    }

    void setParent(Expression * parent) {
      m_parent = parent;
    }

    virtual void visit(TreeWalker * walker) =0;


  };

}
#endif
