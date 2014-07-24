%{
#include BISON_FILE
#include <iostream>
#include <string>
using namespace std;
%}

%option noyywrap

%%

[0-9]+   { yylval.val = atoi(yytext); return NUM; }
"+"  { yylval.sym = yytext[0]; return ADD; }
"-"   { yylval.sym = yytext[0]; return MINUS; }
"*"   { yylval.sym = yytext[0]; return MULTIPLY; }
"/"   { yylval.sym = yytext[0]; return DIVIDE; }
"%"   { yylval.sym = yytext[0]; return MODULO; }

">"   { yylval.sym = yytext[0]; return GT; }
"<"   { yylval.sym = yytext[0]; return LT; }
">="   { yylval.sym = yytext[0]; return GT_EQ; }
"<="   { yylval.sym = yytext[0]; return LT_EQ; }
"=="   { yylval.sym = yytext[0]; return EQUAL; }
"!="   { yylval.sym = yytext[0]; return NE; }

"^"   { yylval.sym = yytext[0]; return XOR; }
"&"   { yylval.sym = yytext[0]; return AND; }
"~"   { yylval.sym = yytext[0]; return OR; }
">>"   { yylval.sym = yytext[0]; return SHIFT_RIGHT; }
"<<"   { yylval.sym = yytext[0]; return SHIFT_LEFT; }

"."   { yylval.sym = yytext[0]; return DOT; }
","   { yylval.sym = yytext[0]; return COMMA; }


[a-zA-Z]+[a-zA-A0-9]* { 
  yylval.str = new std::string(&yytext[0],yyleng); 
  return ID;
}


"("      { return LP; }
")"      { return RP; }
"{"      { return LC; }
"}"      { return RC; }

";"      { return SEMI; }
<<EOF>>  { return 0; }
[ \t\n]+ { }
.        { cerr << "Unrecognized token: " << yytext[0] << " " << yytext << endl; exit(1); }
%%
