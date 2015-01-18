#include "assert.hpp"

#include "SymbolTable.hpp"

#include "llvm/IR/Instructions.h"

//#include <memory>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>

namespace {

}

namespace anvil{


  // private methods
  size_t SymbolTable::getUniqueNumber()
  {
    return m_nameCounter++;
  }

  // public methods

  SymbolTable::SymbolTable() : 
    m_nameCounter(0UL),
    m_currentNames(new NamesMap()),
    m_currentVariables(new VariablesMap()),
    m_currentDefinedFunctions(new FunctionsMap())
  {
    m_names.push_back(m_currentNames);
    m_variables.push_back(m_currentVariables);
    m_definedFunctions.push_back(m_currentDefinedFunctions);
  }

  /* ########## scopes ########## */
  void SymbolTable::descendScope()
  {
    m_currentNames = std::shared_ptr<NamesMap>(new NamesMap());
    m_currentVariables = std::shared_ptr<VariablesMap>(new VariablesMap());
    m_currentDefinedFunctions = std::shared_ptr<FunctionsMap>(new FunctionsMap());

    m_names.push_back(m_currentNames);
    m_variables.push_back(m_currentVariables);
    m_definedFunctions.push_back(m_currentDefinedFunctions);
  }

  void SymbolTable::ascendScope()
  {
    m_currentNames = m_names.back();
    m_currentVariables = m_variables.back();
    m_currentDefinedFunctions = m_definedFunctions.back();

    m_names.pop_back();
    m_variables.pop_back();
    m_definedFunctions.pop_back();    
  }

  /* ########## names ########## */

  std::string SymbolTable::getUniqueName()
  {
    std::ostringstream strs;
    strs << "R" << getUniqueNumber() << "_";
    return strs.str();
  }

  bool SymbolTable::hasName(std::string name) 
  {
    return m_currentNames->count(name) != 0;
  }

  std::string SymbolTable::addName(std::string name)
  {
    ASSERT(!hasName(name),"name exists: " << name);
    
    std::string mappedName = getUniqueName();
    m_currentNames->emplace(name,mappedName);
    
    return mappedName;
  }

  std::string SymbolTable::addOrUpdateName(std::string name)
  {
    m_currentNames->emplace(name,getUniqueName());
    return getName(name);
  }

  std::string SymbolTable::getName(std::string name)
  {
    ASSERT(hasName(name),"name doesn't exist: " << name);
    return m_currentNames->at(name);
  }

  /* ########## variables ########## */


  bool SymbolTable::hasVariable(std::string name)
  {
    return m_currentVariables->count(name) != 0;
  }

  void SymbolTable::storeVariable(std::string name, llvm::AllocaInst * variable)
  {
    m_currentVariables->emplace(name, variable);
  }

  llvm::AllocaInst * SymbolTable::getVariable(std::string name)
  {
    ASSERT(hasVariable(name),"variable doesn't exist: " << name);
    return m_currentVariables->at(name);
  }

  /* ########## defined functions ########## */


  bool SymbolTable::hasFunction(std::string name)
  {
    return m_currentDefinedFunctions->count(name) != 0;
  }

  void SymbolTable::storeFunctionDefinition(std::string name, llvm::Function * functionDefinition)
  {
    m_currentDefinedFunctions->emplace(name, functionDefinition);
  }

  llvm::Function * SymbolTable::getFunctionDefinition(std::string name)
  {
    ASSERT(hasFunction(name),"function definition doesn't name exists: " << name);
    return m_currentDefinedFunctions->at(name);
  }


}

