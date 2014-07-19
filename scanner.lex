%{
#include BISON_FILE
#include <iostream>
using namespace std;
%}

%option noyywrap

%%

[0-9]+   { yylval.val = atoi(yytext); return NUM; }
"+"  { yylval.sym = yytext[0]; return ADD; }
"*"   { yylval.sym = yytext[0]; return MULTIPLY; }
"("      { return LP; }
")"      { return RP; }
";"      { return SEMI; }
<<EOF>>  { return 0; }
[ \t\n]+ { }
.        { cerr << "Unrecognized token!" << endl; exit(1); }
%%
