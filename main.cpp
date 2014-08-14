#include "ast/ast.hpp"
#include "parser/Parser.hpp"


#include <iostream>

#include <stdio.h>




int main(int argc, char **argv)
{

  anvil::Parser parser;
  parser.setFile(argv[1]);
  parser.parse();

  anvil::Node * root = parser.getTreeRoot();

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
