#include "Node/AllNodes.hpp"
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

  anvil::Statement * runner = dynamic_cast<anvil::Statement *>(root);
  while(runner){
    std::cout << ":";
    std::cout << runner->print();
    std::cout << std::endl;
    runner = runner->next();
  }

  return 0;
}
