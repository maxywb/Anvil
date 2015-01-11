#include "ast/ast.hpp"
#include "parser/Parser.hpp"
#include "visitor/TreeWalker.hpp"

#include <iostream>






int main(int argc, char **argv)
{

  anvil::Parser parser;
  parser.setFile(argv[1]);
  parser.parse();

  anvil::TreeWalker treeWalker;



  anvil::StatementList * root = parser.getTreeRoot();

  anvil::StatementList * stmtList = dynamic_cast<anvil::StatementList *>(root);
#ifdef DEBUG
  for(anvil::StatementList::iterator itr = stmtList->begin();
      itr != stmtList->end();
      itr++){
      std::cout << ":";
      std::cout << (*itr)->print();
      std::cout << std::endl;
  }
#endif
  treeWalker.visit(root);





  return 0;
}
