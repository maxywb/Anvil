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
  {
  }

  NameGenerator::~NameGenerator() 
  {
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

  std::shared_ptr<SymbolTable> SymbolTable::parent() const
  {
    return m_parent.lock();
  }

  // public methods

  SymbolTable::SymbolTable(NameGenerator & nameGen) : 
    m_parent(),
    m_children(),
    m_captures(),
    m_names(),
    m_variables(),
    m_definedFunctions(),
    m_nameGenerator(nameGen)
  {
  }

  void SymbolTable::setParent(std::shared_ptr<SymbolTable> newParent)
  {
    m_parent = newParent;
  }

  std::shared_ptr<SymbolTable> SymbolTable::getParent() 
  {
    return m_parent.lock();
  }

  std::shared_ptr<SymbolTable> SymbolTable::makeChild()
  {
    std::shared_ptr<SymbolTable> newTable(new SymbolTable(m_nameGenerator));
    newTable->setParent(shared_from_this());

    m_children.push_back(newTable);    

    return newTable;
  }

  std::list<std::shared_ptr<SymbolTable>> const & SymbolTable::getChildren()
  {
    return m_children;
  }

  /* ########## names ########## */

  bool const SymbolTable::hasName(std::string name) const
  {
    bool const foundIt = m_names.count(name);

    if (foundIt) {
      return true;
    } else if (m_parent.use_count() > 0) {
      return parent()->hasName(name);
    } else {
      return false;
    }
  }

  std::string SymbolTable::addName(std::string name)
  {
    ASSERT(!hasName(name),"name exists: " << name);
    
    std::string mappedName = m_nameGenerator.getUniqueName();
    m_names.emplace(name,mappedName);
    
    return mappedName;
  }

  std::string const SymbolTable::getName(std::string name) 
  {
    ASSERT(hasName(name),"name doesn't exist: " << name);

    if (m_names.count(name)) {
      return m_names[name];
    } else {
      return parent()->getName(name);
    }
  }

  /* ########## variables ########## */

  bool const SymbolTable::hasVariable(std::string name) const
  {
    bool const foundIt = m_variables.count(name) != 0;      

    if (foundIt) {
      return true;
    } else if (m_parent.use_count() > 0) {
      return parent()->hasVariable(name);
    } else {
      return false;
    }
  }

  void SymbolTable::storeVariable(std::string name, llvm::AllocaInst * variable)
  {
    m_variables.emplace(name, variable);
  }

  llvm::AllocaInst * SymbolTable::getVariable(std::string name) 
  {
    ASSERT(hasVariable(name),"variable doesn't exist: " << name);

    if (m_variables.count(name)) {
      return m_variables[name];
    } else {
      return parent()->getVariable(name);
    }
  }

  /* ########## defined functions ########## */

  bool const SymbolTable::hasFunction(std::string name) const
  {
    bool const foundIt = m_definedFunctions.count(name) != 0;      

    if (foundIt) {
      return true;
    } else if (m_parent.use_count() > 0) {
      return parent()->hasFunction(name);
    } else {
      return false;
    }
  }

  void SymbolTable::storeFunctionDefinition(std::string name, llvm::Function * functionDefinition)
  {
    m_definedFunctions.emplace(name, functionDefinition);
  }

  llvm::Function * SymbolTable::getFunctionDefinition(std::string name) 
  {
    ASSERT(hasFunction(name),"function definition doesn't exist: " << name);
    return m_definedFunctions.at(name);
  }

  /* ########## defined captures ########## */

  bool const SymbolTable::hasCapture(std::string name) const
  {
    bool const foundIt = m_definedCaptures.count(name) != 0;      

    if (foundIt) {
      return true;
    } else if (m_parent.use_count() > 0) {
      return parent()->hasCapture(name);
    } else {
      return false;
    }
  }

  void SymbolTable::storeCaptureDefinition(std::string name, llvm::Function * captureDefinition)
  {
    m_definedCaptures.emplace(name, captureDefinition);
  }

  llvm::Function * SymbolTable::getCaptureDefinition(std::string name) 
  {
    ASSERT(hasCapture(name),"capture definition doesn't exist: " << name);
    return m_definedCaptures.at(name);
  }


}

