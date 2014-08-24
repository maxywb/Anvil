#include "Term.hpp"

#include <string>
#include <sstream>
#include <list>
#include <ostream>



namespace anvil{


  std::string Term::print()
  {
    std::ostringstream strs;
      
    strs << m_operation << " ";

    for(std::list<std::string>::const_iterator itr = m_operands.begin();
	itr != m_operands.end();
	itr++) {
	
      strs << *itr << " ";

    }


    return strs.str();
  }


}


