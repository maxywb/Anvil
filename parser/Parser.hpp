#ifndef ANVIL_PARSER_HPP
#define ANVIL_PARSER_HPP

#include "ast/ast.hpp"
#include "assert.hpp"

//#include BISON_FILE

#include <iostream>

#include <stdio.h>

extern int yyparse();
extern FILE * yyin;
extern anvil::StatementList * s_root;

namespace anvil{
  class Parser 
  {
  private:
    StatementList  * m_treeRoot;
    
  public:
    void setFile(char * file);

    void parse();

    StatementList * getTreeRoot();

  };


}
#endif
