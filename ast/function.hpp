#ifndef ANVIL_FUNCTION_HPP
#define ANVIL_FUNCTION_HPP

#include "node.hpp"
#include "statements.hpp"

#include <string>
#include <sstream>
#include <ostream>
#include <list>

namespace anvil{


  class FunctionDefinition : public Statement
  {
  private:
    std::string m_name;
    std::list<Id *> m_parameters;

    StatementList * m_body;

  public:
    FunctionDefinition(std::string * name, std::list<Id *> * params, 
		       StatementList * body) 
      : m_name(*name), m_parameters(params->begin(),params->end()), m_body(body){}

    std::string print(){
      std::ostringstream strs;

      strs << "def ";
      strs << m_name;
      strs << "(";
      for(auto itr : m_parameters) {
	strs << itr->print();
	strs << ",";
      }
      strs << ")";
      strs << std::endl << "{" << std::endl;

      for(StatementList::iterator itr = m_body->begin();
	  itr != m_body->end();
	  itr++){
	strs << ":";
	strs << (*itr)->print();
	strs << std::endl;
      }
      strs << "}";

      return strs.str();
    }

    void visit(TreeWalker * walker)
    {
      walker->visit(this);
    }

    StatementList * getBody() 
    {
      return m_body;
    }

    std::string getName()
    {
      return m_name;
    }

    std::list<Id *> getParameters()
    {
      return m_parameters;
    }

  };



}
#endif
