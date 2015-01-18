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

  std::list<anvil::Statement *> statements(parser.getTreeRoot()->begin(),
					   parser.getTreeRoot()->end());

#ifdef DEBUG
  for(auto line : statements) {
    std::cout << ": ";
    std::cout << line->print();
    std::cout << std::endl;
  }
#endif

  for(auto stmt : statements) {
    treeWalker.visit(stmt);
  }

  return 0;
}
