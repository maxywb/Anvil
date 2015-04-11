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
#include <utility>

namespace anvil{

  struct FunctionCapture {
      std::pair<std::string,std::string> const names;
      llvm::Function const * containingFunction;

    FunctionCapture(std::string const uniqueName, std::string const name, llvm::Function const * _containingFunction)
          : names(uniqueName, name), containingFunction(_containingFunction)
      {}
  };


  class NameGenerator
  {
  private:
    size_t m_nameCounter;
    size_t getUniqueNumber();
    
  public:
    NameGenerator();
    ~NameGenerator();
    std::string getUniqueName();
  };

  /*
   * Represents the name resolution allowed in a given function scope.
   */
  class SymbolTable
  {
  private:
    typedef std::unordered_map<std::string, std::string> NamesMap;
    typedef std::unordered_map<std::string, llvm::AllocaInst * > VariablesMap;
    typedef std::unordered_map<std::string, llvm::Function *> FunctionsMap;

    std::weak_ptr<SymbolTable> m_parent;
    std::list<std::shared_ptr<SymbolTable>> m_children;
    std::list<FunctionCapture> m_captures;
    
    std::list<std::shared_ptr<NamesMap>> m_names;
    std::shared_ptr<NamesMap> m_currentNames;

    std::list<std::shared_ptr<VariablesMap>> m_variables;
    std::shared_ptr<VariablesMap> m_currentVariables;

    std::list<std::shared_ptr<FunctionsMap>> m_definedFunctions;
    std::shared_ptr<FunctionsMap> m_currentDefinedFunctions;

    std::list<std::shared_ptr<VariablesMap>> m_capturedVariables;
    std::shared_ptr<VariablesMap> m_currentCapturedVariables;

    NameGenerator & m_nameGenerator;

  public:

    SymbolTable(std::weak_ptr<SymbolTable> parent, NameGenerator & nameGen);

    /* ########## scopes ########## */

    void descendScope();
    void ascendScope();

    /* ########## names ########## */

    bool hasName(std::string name);

    std::string addName(std::string name);
    
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
