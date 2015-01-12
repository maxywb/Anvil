#ifndef ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP
#define ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP

#include "assert.hpp"

#include "llvm/IR/Instructions.h"

#include <stack>
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

    std::unordered_map<std::string, std::string> m_names;
    std::unordered_map<std::string, llvm::AllocaInst *> m_variables;
    std::unordered_map<std::string, llvm::Function *> m_definedFunctions;

    size_t getUniqueNumber()
    {
      return m_nameCounter++;
    }

  public:

    SymbolTable() : m_nameCounter(0UL) {}

    /* ########## names ########## */

    std::string getUniqueName()
    {
      std::ostringstream strs;
      strs << "R" << getUniqueNumber() << "_";
      return strs.str();
    }

    bool hasName(std::string name) 
    {
      return m_names.count(name) != 0;
    }

    std::string addName(std::string name)
    {
      ASSERT(!hasName(name),"name exists: " << name);
      
      return m_names[name] = getUniqueName();
    }

    std::string addOrUpdateName(std::string name)
    {
      m_names[name] = getUniqueName();
      return getName(name);
    }

    std::string getName(std::string name)
    {
      ASSERT(hasName(name),"name exists: " << name);
      return m_names[name];
    }

    /* ########## variables ########## */


    bool hasVariable(std::string name) {
      return m_variables.count(name) != 0;
    }

    void storeVariable(std::string name, llvm::AllocaInst * variable) {
      m_variables[name] = variable;
    }

    llvm::AllocaInst * getVariable(std::string name) {
      ASSERT(hasVariable(name),"variable doesn't exist: " << name);
      return m_variables[name];
    }

    /* ########## defined functions ########## */


    bool hasFunction(std::string name) {
      return m_definedFunctions.count(name) != 0;
    }

    void storeFunctionDefinition(std::string name, llvm::Function * functionDefinition) {
      m_definedFunctions[name] = functionDefinition;
    }

    llvm::Function * getFunctionDefinition(std::string name) {
      ASSERT(hasFunction(name),"function definition doesn't name exists: " << name);
      return m_definedFunctions[name];
    }

  };
}
#endif
