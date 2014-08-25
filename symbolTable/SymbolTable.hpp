#ifndef ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP
#define ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP

#include "assert.hpp"

#include <string>
#include <sstream>
#include <unordered_map>

namespace anvil{


  class SymbolTable
  {
  private:
    size_t m_nameCounter;

    std::unordered_map<std::string,size_t> m_symbolMap;

    size_t getUniqueNumber()
    {
      return m_nameCounter++;
    }

  public:

    SymbolTable() : m_nameCounter(0UL) {}

    std::string getUniqueName()
    {
      std::ostringstream strs;
      strs << "temp" << getUniqueNumber();
      return strs.str();
    }


    size_t addSymbol(std::string symbol)
    {
      ASSERT(m_symbolMap.count(symbol) == 0,"symbol exists: " << symbol);
      m_symbolMap[symbol] = getUniqueNumber();
    }

    std::string addName(std::string symbol)
    {
      ASSERT(m_symbolMap.count(symbol) == 0,"symbol exists: " << symbol);
      addSymbol(symbol);
      return getName(symbol);
    }

    std::string addOrUpdateName(std::string symbol)
    {
      m_symbolMap[symbol] = getUniqueNumber();
      return getName(symbol);
    }

    size_t getSymbol(std::string symbol)
    {
      ASSERT(m_symbolMap.count(symbol) != 0,"symbol exists: " << symbol);
      return m_symbolMap[symbol];
    }

    std::string getName(std::string symbol)
    {
      ASSERT(m_symbolMap.count(symbol) != 0,"symbol exists: " << symbol);
      std::ostringstream strs;
      strs << "temp" << m_symbolMap[symbol];
      return strs.str();
    }

    size_t updateSymbol(std::string symbol)
    {
      ASSERT(m_symbolMap.count(symbol) != 0,"can't update non-existant symbol: " << symbol);
      m_symbolMap[symbol] = getUniqueNumber();
    }



  };
}
#endif
