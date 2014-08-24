#ifndef ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP
#define ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP

#include <string>
#include <sstream>

namespace anvil{


  class SymbolTable
  {
  private:
    size_t m_nameCounter;


  public:

    SymbolTable() : m_nameCounter(0UL) {}

    std::string getUniqueName()
    {
      std::ostringstream strs;
      strs << "temp" << m_nameCounter++;
      return strs.str();
    }
  };
}
#endif
