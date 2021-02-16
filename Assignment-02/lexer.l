%{
#include <stdio.h>
#include <string.h>
%}

pre_process ^#(.)*
line_comment    \/\/(.)*
multi_comment   \/\*(.|\n)*\*\/

keyword     auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while

id          [a-zA-Z_]([a-zA-Z0-9_])*
function    {id}\((.)*\)

realConst (\+|\-)?[1-9][0-9]*\.[0-9]+ 
intConst (\+|\-)?[1-9][0-9]*
charConst   \'[a-zA-Z]\'
stringConst    \"[a-z A-Z]*\"

assignOp =
bitwiseOp "^"|"&"|"|"|"<<"|">>" 
arithAssignOp "+="|"-="|"*="|"\="|"%="
relOp <|<=|>|>=|=|!= 
arithOp "+"|"-"|"*"|"/"|"%" 
logicOp &&|\|\||! 
separators ";"|","|"."|"["|"]"|"("|")"|"{"|"}"|"["|"]"

/*printf("  | %25s | %-25s |\n", yytext, "Function call");*/
%%
{keyword}       {printf("  | %25s | %-25s |\n", yytext, "Keyword");}
{function}      {printf("  | %25s | %-25s |\n", yytext, "Function call");}
{id}            {printf("  | %25s | %-25s |\n", yytext, "Identifier");}
{realConst}     {printf("  | %25s | %-25s |\n", yytext, "Real const");}
{intConst}      {printf("  | %25s | %-25s |\n", yytext, "Integer Constant");}
{bitwiseOp}     {printf("  | %25s | %-25s |\n", yytext, "Bitwise Operator");}
{assignOp}      {printf("  | %25s | %-25s |\n", yytext, "Assignment Operator");}
{arithAssignOp} {printf("  | %25s | %-25s |\n", yytext, "Arith Assign Operator");}
{arithOp}       {printf("  | %25s | %-25s |\n", yytext, "Arithmetic Operator");}
{logicOp}       {printf("  | %25s | %-25s |\n", yytext, "Logical Operator");}
{relOp}         {printf("  | %25s | %-25s |\n", yytext, "Relational Operator");}
{charConst}     {printf("  | %25s | %-25s |\n", yytext, "Character Constant");}
{stringConst}   {printf("  | %25s | %-25s |\n", yytext, "String Constant");}
{separators}    {printf("  | %25s | %-25s |\n", yytext, "Seperators");}
{pre_process}   {printf("  | %25s | %-25s |\n", yytext, "Preprocessor Directive");}
{line_comment}  {printf("  | %25s | %-25s |\n", yytext, "Line comment");}
{multi_comment} {
    char *lines = strtok(yytext, "\n");
    while(lines){
        printf("  | %25s | ", lines);
        lines = strtok(NULL, "\n");
        printf("%-25s |\n",(lines!=NULL)?" ": "Multiline Comment");}
    }
.|\n {}
%%
int main(int argc, char **argv)
{ 
    if(argc != 2){
        fprintf(stderr,"Please Enter file as second argument!\n");
        return 1;
    }
    yyin = fopen(argv[1], "rt");
    if(yyin == NULL){
        fprintf(stderr,"File not found!\n");
        return 1;
    }
    printf("  +---------------------------+---------------------------+\n");
    yylex();
    printf("  +---------------------------+---------------------------+\n");

}
