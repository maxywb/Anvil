#include "assert.hpp"

#include "SymbolTable.hpp"

#include "llvm/IR/Instructions.h"

#include <memory>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>

namespace {

}

namespace anvil{

  NameGenerator::NameGenerator() :
    m_nameCounter(0UL)
  {}
  NameGenerator::~NameGenerator() 
  {
    return;
  }

  size_t NameGenerator::getUniqueNumber()
  {
    return m_nameCounter++;
  }

  std::string NameGenerator::getUniqueName()
  {
    std::ostringstream strs;
    strs << "R" << getUniqueNumber() << "_";
    return strs.str();
  }



  // public methods

  SymbolTable::SymbolTable(std::weak_ptr<SymbolTable> parent, NameGenerator & nameGen) : 
    m_parent(parent),
    m_children(),
    m_captures(),
    m_currentNames(new NamesMap()),
    m_currentVariables(new VariablesMap()),
    m_currentDefinedFunctions(new FunctionsMap()),
    m_nameGenerator(nameGen)
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
    m_currentCapturedVariables = std::shared_ptr<VariablesMap>(new VariablesMap());

    m_names.push_back(m_currentNames);
    m_variables.push_back(m_currentVariables);
    m_definedFunctions.push_back(m_currentDefinedFunctions);
    m_capturedVariables.push_back(m_currentCapturedVariables);
  }

  void SymbolTable::ascendScope()
  {
    m_names.pop_back();
    m_variables.pop_back();
    m_definedFunctions.pop_back();    
    m_capturedVariables.pop_back();

    m_currentNames = m_names.back();
    m_currentVariables = m_variables.back();
    m_currentDefinedFunctions = m_definedFunctions.back();
    m_currentCapturedVariables = m_variables.back();
  }

  /* ########## names ########## */

  bool SymbolTable::hasName(std::string name) 
  {
    for (auto namesRunner = m_names.crbegin();
	 namesRunner != m_names.crend(); 
	 ++namesRunner) {
      
      if ((*namesRunner)->count(name) != 0) { // dereference iterator 
	return true;
      }

    }
    
    return false;
  }

  std::string SymbolTable::addName(std::string name)
  {
    ASSERT(!hasName(name),"name exists: " << name);
    
    std::string mappedName = m_nameGenerator.getUniqueName();
    m_currentNames->emplace(name,mappedName);
    
    return mappedName;
  }

  std::string SymbolTable::getName(std::string name)
  {
    ASSERT(hasName(name),"name doesn't exist: " << name);

    for (auto namesRunner = m_names.crbegin();
	 namesRunner != m_names.crend(); 
	 ++namesRunner) {
      
      if ((*namesRunner)->count(name) != 0) { // dereference iterator 
	return (*namesRunner)->at(name); // dereference iterator 
      }

    }

    // can't happen due to assertion above
    ASSERT(false, "unknown getName failure");

    return std::string();
  }

  /* ########## variables ########## */


  bool SymbolTable::hasVariable(std::string name)
  {
    for (auto variablesRunner = m_variables.crbegin();
	 variablesRunner != m_variables.crend(); 
	 ++variablesRunner) {
      
      if ((*variablesRunner)->count(name) != 0) { // dereference iterator 
	return true;
      }

    }
    
    return false;

  }

  void SymbolTable::storeVariable(std::string name, llvm::AllocaInst * variable)
  {
    m_currentVariables->emplace(name, variable);
  }

  llvm::AllocaInst * SymbolTable::getVariable(std::string name)
  {
    ASSERT(hasVariable(name),"variable doesn't exist: " << name);

    bool checkingUpScope = false;

    llvm::AllocaInst * foundVariable = nullptr;

    for (auto variablesRunner = m_variables.crbegin();
	 variablesRunner != m_variables.crend(); 
	 ++variablesRunner) {
      
      if ((*variablesRunner)->count(name) != 0) { // dereference iterator 
	foundVariable =  (*variablesRunner)->at(name); // dereference iterator 

	if (checkingUpScope) {
	  m_currentCapturedVariables->emplace(name, foundVariable);
	}

	break;
      }

      checkingUpScope = true;
    }

    return foundVariable;
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
    ASSERT(hasFunction(name),"function definition doesn't exist: " << name);
    return m_currentDefinedFunctions->at(name);
  }


}

