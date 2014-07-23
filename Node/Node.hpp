#ifndef ANVIL_NODES_HPP
#define ANVIL_NODES_HPP

#include "operators.hpp"
#include "Expression.hpp"

#include <string>

namespace anvil{

  class Node 
  {
  public:
    virtual std::string print(){
      return "node";
    }


    Node(){}
  };


}
#endif
