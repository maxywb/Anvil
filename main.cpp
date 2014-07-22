#include BISON_FILE
#include "Nodes.hpp"

extern int yyparse();
int main(int argc, char **argv)
{

  yyparse();

  return 0;
}
