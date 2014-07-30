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

    Statement * m_bodyStart;

  public:
    FunctionDefinition(std::string * name, std::list<std::string *> * params, 
		       Statement * bodyStart) 
      : m_name(name), m_parameters(params), m_bodyStart(bodyStart){}

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
      anvil::Statement * runner = m_bodyStart;
      while(runner){
	strs << ":";
	strs << runner->print();
	strs << std::endl;
	runner = runner->next();
      }
      strs << "}";

    return strs.str();
  }

};



}
#endif
