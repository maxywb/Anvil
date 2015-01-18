#ifndef ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP
#define ANVIL_SYMBOLTABLE_SYMBOLTABLE_HPP

namespace llvm {
  class AllocaInst;
  class Function;
}

#include <string>
#include <unordered_map>
#include <memory>
#include <list>

namespace anvil{


  class SymbolTable
  {
  private:
    typedef std::unordered_map<std::string, std::string> NamesMap;

    typedef std::unordered_map<std::string, llvm::AllocaInst * > VariablesMap;

    typedef std::unordered_map<std::string, llvm::Function *> FunctionsMap;

    
    size_t m_nameCounter;

    std::list<std::shared_ptr<NamesMap>> m_names;
    std::shared_ptr<NamesMap> m_currentNames;

    std::list<std::shared_ptr<VariablesMap>> m_variables;
    std::shared_ptr<VariablesMap> m_currentVariables;

    std::list<std::shared_ptr<FunctionsMap>> m_definedFunctions;
    std::shared_ptr<FunctionsMap> m_currentDefinedFunctions;

    size_t getUniqueNumber();

  public:

    SymbolTable();

    /* ########## scopes ########## */
    void descendScope();
    void ascendScope();

    /* ########## names ########## */

    std::string getUniqueName();

    bool hasName(std::string name);

    std::string addName(std::string name);
    
    std::string addOrUpdateName(std::string name);

    std::string getName(std::string name);

    /* ########## variables ########## */


    bool hasVariable(std::string name);

    void storeVariable(std::string name, llvm::AllocaInst * variable);

    llvm::AllocaInst * getVariable(std::string name);

    /* ########## defined functions ########## */


    bool hasFunction(std::string name);

    void storeFunctionDefinition(std::string name, llvm::Function * functionDefinition);

    llvm::Function * getFunctionDefinition(std::string name);

  };
}
#endif
