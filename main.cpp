#include "Node/AllNodes.hpp"
#include BISON_FILE


extern int yyparse();

int main(int argc, char **argv)
{

  yyparse();

  return 0;
}
