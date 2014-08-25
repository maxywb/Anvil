#include "ast/ast.hpp"
#include "parser/Parser.hpp"
#include "visitor/TreeWalker.hpp"

#include <iostream>

#include <stdio.h>




int main(int argc, char **argv)
{

  anvil::Parser parser;
  parser.setFile(argv[1]);
  parser.parse();

  anvil::TreeWalker treeWalker;



  anvil::StatementList * root = parser.getTreeRoot();

  anvil::StatementList * stmtList = dynamic_cast<anvil::StatementList *>(root);
  for(anvil::StatementList::iterator itr = stmtList->begin();
      itr != stmtList->end();
      itr++){
      std::cout << ":";
      std::cout << (*itr)->print();
      std::cout << std::endl;
  }

  treeWalker.visit(root);

  std::cout << std::endl;

  treeWalker.printTerms();
  return 0;
}
