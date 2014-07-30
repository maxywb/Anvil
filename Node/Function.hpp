#ifndef ANVIL_FUNCTION_HPP
#define ANVIL_FUNCTION_HPP

#include "Node.hpp"
#include "Statement.hpp"

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

  public:
    FunctionDefinition(std::string * name, std::list<std::string *> * params) 
      : m_name(name), m_parameters(params) {}

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



      return strs.str();
    }

  };



}
#endif
