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
    std::string * m_name;
    std::list<std::string *> * m_parameters;

    StatementList * m_body;

  public:
    FunctionDefinition(std::string * name, std::list<std::string *> * params, 
		       StatementList * body) 
        : m_name(name), m_parameters(params), m_body(body){}

    std::string print(){
      std::ostringstream strs;

      strs << "def ";
      strs << *m_name;
      strs << "(";
      for(std::list<std::string *>::iterator itr = m_parameters->begin();
	  itr != m_parameters->end();
	  itr++){
	strs << *(*itr);
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

};



}
#endif