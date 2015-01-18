#ifndef ANVIL_PARSER_HPP
#define ANVIL_PARSER_HPP

#include "ast/ast.hpp"
#include "assert.hpp"

//#include BISON_FILE

#include <iostream>
#include <list>

#include <stdio.h>

extern int yyparse();
extern FILE * yyin;
extern std::list<anvil::Statement * > * s_root;

namespace anvil{
  class Parser 
  {
  private:
    std::list<Statement  *> * m_treeRoot;
    
  public:
    void setFile(char * file);

    void parse();

    std::list<Statement *> * getTreeRoot();

  };


}
#endif
