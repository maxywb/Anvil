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
  class SymbolTable : public std::enable_shared_from_this<SymbolTable>
  {
  private:
    typedef std::unordered_map<std::string, std::string> NamesMap;
    typedef std::unordered_map<std::string, llvm::AllocaInst * > VariablesMap;
    typedef std::unordered_map<std::string, llvm::Function *> FunctionsMap;

    std::weak_ptr<SymbolTable> m_parent;
    std::list<std::shared_ptr<SymbolTable>> m_children;
    std::list<std::string> m_captures;
    
    NamesMap m_names;

    VariablesMap m_variables;

    FunctionsMap m_definedFunctions;

    VariablesMap m_capturedVariables;

    NameGenerator & m_nameGenerator;

    std::shared_ptr<SymbolTable> parent() const;

  public:

    SymbolTable(NameGenerator & nameGen);

    void setParent(std::shared_ptr<SymbolTable> newParent);
    std::shared_ptr<SymbolTable> getParent();

    std::shared_ptr<SymbolTable> makeChild();
    std::list<std::shared_ptr<SymbolTable>> const & getChildren();

    /* ########## names ########## */

    bool const hasName(std::string name) const;

    std::string addName(std::string name);
    
    std::string const getName(std::string name);

    /* ########## variables ########## */

    bool const hasVariable(std::string name) const;

    void storeVariable(std::string name, llvm::AllocaInst * variable);

    llvm::AllocaInst * getVariable(std::string name);

    /* ########## defined functions ########## */


    bool const hasFunction(std::string name) const;

    void storeFunctionDefinition(std::string name, llvm::Function * functionDefinition);

    llvm::Function * getFunctionDefinition(std::string name);

    /* ########## defined captures ########## */

    void addCapture(std::string name);

    void propagateCaptures(std::string name);

    llvm::Function * getCaptureDefinition(std::string name);

  };
}
#endif
