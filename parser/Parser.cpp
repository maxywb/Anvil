
#include "ast/ast.hpp"
#include "assert.hpp"
#include "Parser.hpp"

#include BISON_FILE

#include <iostream>

#include <stdio.h>

extern int yyparse();
extern FILE * yyin;
extern anvil::Node * s_root;

namespace anvil{
  void Parser::setFile(char * file)
    {
      yyin = fopen(file,"r");

      ASSERT(yyin, "can't open file");
    }

  void Parser::parse()
    {
      ASSERT(yyin, "must call setFile before parsing");
      yyparse();
      m_treeRoot = s_root;
    }

  Node * Parser::getTreeRoot()
    {
      return s_root;
    }

}
