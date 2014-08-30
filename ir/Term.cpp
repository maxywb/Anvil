#include "Term.hpp"
#include "assert.hpp"

#include <string>
#include <sstream>
#include <list>
#include <ostream>



namespace anvil{


  std::string Term::print()
  {
    std::ostringstream strs;
    
    switch(m_operation){
      case operations::Add:
        strs << "add ";
        break;
      case operations::Multiply:
        strs << "mult ";
        break;
      case operations::LessThan:
        strs << "lt ";
        break;
      case operations::Jump:
        strs << "jump ";
        break;
      case operations::JumpIfEqual:
        strs << "jump_eq ";
        break;
      case operations::Label:
        strs << "label ";
        break;
      default:
        ASSERT(false, "unsupported operation");
    }

    for(std::vector<std::string>::const_iterator itr = m_operands.begin();
	itr != m_operands.end();
	itr++) {
	
      strs << *itr << " ";

    }


    return strs.str();
  }


}


