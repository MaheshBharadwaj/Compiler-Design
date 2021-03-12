%{
    #include <stdlib.h>
    #include <stdio.h>
    int yylex(void);
    extern FILE* yyin;
    #include "y.tab.h"

    int power(int a, int b){
        int prod = 1;
        for(int i = 0;i< b;i++)
            prod*=a;
        return prod;
  }

%}

%token INT AND OR NOT LSHIFT RSHIFT

%%
program : line program
        | line

line    : expr '\n' {printf("Result: %d\n", $1); }

expr    : expr '+' mulexpr { $$ = $1 + $3; }
        | expr '-' mulexpr { $$ = $1 - $3; }
        | mulexpr { $$ = $1;}

mulexpr : mulexpr '*' powexpr { $$ = $1 * $3; }
        | mulexpr '/' powexpr { $$ = $1 / $3; }
        | powexpr { $$ = $1; }

powexpr : powexpr '^' boolexpr {$$ = power($1, $3);}
        | boolexpr { $$ = $1;}

boolexpr: boolexpr AND bitexpr {$$ = ($1 & $3)? 1:0;}
        | boolexpr OR bitexpr {$$ = ($1 | $3)? 1:0;}
        | NOT boolexpr {$$ = (!$2)? 1:0;}
        | bitexpr {$$ = $1;}

bitexpr : bitexpr LSHIFT term {$$ = $1 << $3;}
        | bitexpr RSHIFT term {$$ = $1 >> $3;}
        | term {$$ = $1;}

term    : '(' expr ')' { $$ = $2; }
        | INT { $$ = $1; }
%%
int yyerror(char* str){
    fprintf(stderr, "%s\n", str);
    return 0;
}
int yywrap(){
    return 1;
}

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "Enter file name as argument!\n");
        return 1;
    }
    yyin = fopen(argv[1], "rt");
    if (!yyin){
        fprintf(stderr, "File not found!\n");
        return 2;
    }
    yyparse();
    return 0;
}
