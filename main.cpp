#include "ast/ast.hpp"

#include BISON_FILE

#include <iostream>

#include <stdio.h>

extern int yyparse();
extern FILE * yyin;
extern anvil::Node * root;



int main(int argc, char **argv)
{
  FILE * fp = fopen(argv[1],"r");
  yyin = fp;

  yyparse();

  anvil::StatementList * stmtList = dynamic_cast<anvil::StatementList *>(root);
  for(anvil::StatementList::iterator itr = stmtList->begin();
      itr != stmtList->end();
      itr++){
      std::cout << ":";
      std::cout << (*itr)->print();
      std::cout << std::endl;
  }

  return 0;
}
