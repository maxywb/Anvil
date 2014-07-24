#ifndef ANVIL_STATEMENT_HPP
#define ANVIL_STATEMENT_HPP

#include "Node.hpp"
#include "Expression.hpp"

#include <list>
#include <string>

#include <sstream>

namespace anvil{
  

  class Statement : public Node
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
};


}
#endif
