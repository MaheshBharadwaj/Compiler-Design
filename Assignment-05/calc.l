%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include "y.tab.h"
    void yyerror(char*);
    extern int yylval;
%}
%%
[ \t]+ ;
[0-9]+ {
  yylval = atoi(yytext); 
  return INT;
}

[+\-^*/] {return *yytext;}
"(" { return *yytext; }
")" { return *yytext; }
"||" { return OR; }
"&&" { return AND; }
"!"  { return NOT; }
"<<" { return LSHIFT; }
">>" { return RSHIFT; }
\n { return *yytext; }

. {
    char err[25] = {0};
    sprintf(err, "Invalid Character \"%s\"\n", yytext);
    yyerror(err);
}
%%