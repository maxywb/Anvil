#ifndef ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP
#define ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP

#include "assert.hpp"

#include "llvm/IR/Instructions.h"

#include <string>
#include <sstream>
#include <unordered_map>

namespace {

}

namespace anvil{


  class SymbolTable
  {
  private:
    size_t m_nameCounter;

    std::unordered_map<std::string,std::string> m_nameMap;
    std::unordered_map<std::string, llvm::AllocaInst *> m_variableMap;

    size_t getUniqueNumber()
    {
      return m_nameCounter++;
    }

  public:

    SymbolTable() : m_nameCounter(0UL) {}

    std::string getUniqueName()
    {
      std::ostringstream strs;
      strs << "R" << getUniqueNumber() << "_";
      return strs.str();
    }

    std::string addName(std::string symbol)
    {
      ASSERT(m_nameMap.count(symbol) == 0,"symbol exists: " << symbol);
      addName(symbol);
      return getName(symbol);
    }

    std::string addOrUpdateName(std::string symbol)
    {
      m_nameMap[symbol] = getUniqueName();
      return getName(symbol);
    }

    std::string getName(std::string symbol)
    {
      ASSERT(m_nameMap.count(symbol) != 0,"symbol exists: " << symbol);
      return m_nameMap[symbol];
    }

    bool hasName(std::string symbol) 
    {
      return m_nameMap.count(symbol) != 0;
    }

    void storeVariable(std::string symbol, llvm::AllocaInst * variable) {
      m_variableMap[symbol] = variable;
    }

    llvm::AllocaInst * getVariable(std::string symbol) {
      ASSERT(m_variableMap.count(symbol) != 0,"symbol exists: " << symbol);
      return m_variableMap[symbol];
    }

    bool hasVariable(std::string symbol) {
      return m_variableMap.count(symbol) != 0;
    }

  };
}
#endif
