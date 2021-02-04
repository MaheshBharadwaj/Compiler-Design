#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

void remove_newline(char *const string)
{
    int i = 0;
    for (; string[i]; i++)
        if (string[i] == '\n')
            string[i] = 0;
    string[i] = 0;
}

int symtab_index = 0;
int start_sno = 1;
int start_addr = 1000;
char buffer_id[MAX] = {0};
char buffer_val[MAX] = {0};
char buffer_type[MAX] = {0};

token_type identify_token(const char *const token)
{
    int tmp;
    token_type type;
    if (token[0] == ' ' || token[0] == '\t' || token[0] == '\n')
        type = WHITESPACE;
    else if (check(token, keywords))
    {
        type = KEYWORD;
        for (int i = 0; i < MAX && data_type[i][0]; i++)
        {
            if (strcmp(token, data_type[i]) == 0)
            {
                strcpy(buffer_type, data_type[i]);
                break;
            }
        }
    }
    else if (check_char_const(token))
    {
        type = CHAR_CONST;
        strcpy(buffer_val, token);
        if (!check_identifier_table(buffer_id, symbol_table))
        {
            symbol_table[symtab_index].sno = start_sno++;
            strcpy(symbol_table[symtab_index].id, buffer_id);
            strcpy(symbol_table[symtab_index].val, buffer_val);
            strcpy(symbol_table[symtab_index].type, buffer_type);
            symbol_table[symtab_index].addr = start_addr;
            if (strcmp(symbol_table[symtab_index].type, "char") == 0)
                start_addr += strlen(symbol_table[symtab_index].val) - 2;
            else if (strcmp(symbol_table[symtab_index].type, "int") == 0)
                start_addr += 2;
            else
                start_addr += 4;
            symtab_index++;
        }
    }
    else if (check_function(token))
        type = FUNCTION;
    else if (check(token, arith_op))
        type = ARITH_OP;
    else if (check(token, arith_assign_op))
        type = ARTITH_ASSIGN_OP;
    else if (check(token, logic_op))
        type = LOGIC_OP;
    else if (check(token, bit_op))
        type = BIT_OP;
    else if (check(token, rel_op))
        type = REL_OP;
    else if (check(token, unary_op))
        type = UNARY_OP;
    else if (check(token, assign_op))
        type = ASSIGN_OP;
    else if (check(token, special))
        type = SPECIAL;
    else if ((tmp = check_real_const(token)) != -1)
    {
        if (tmp == 0)
        {
            type = INT_CONST;
            strcpy(buffer_val, token);
        }
        else
        {
            type = FLOAT_CONST;
            strcpy(buffer_val, token);
        }
        if (!check_identifier_table(buffer_id, symbol_table))
        {
            symbol_table[symtab_index].sno = start_sno++;
            strcpy(symbol_table[symtab_index].id, buffer_id);
            strcpy(symbol_table[symtab_index].val, buffer_val);
            strcpy(symbol_table[symtab_index].type, buffer_type);
            symbol_table[symtab_index].addr = start_addr;
            if (strcmp(symbol_table[symtab_index].type, "char") == 0)
                start_addr += strlen(symbol_table[symtab_index].val) - 2;
            else if (strcmp(symbol_table[symtab_index].type, "int") == 0)
                start_addr += 2;
            else
                start_addr += 4;
            symtab_index++;
        }
    }
    else if (check_indentifier(token))
    {
        type = IDENTIFIER;
        strcpy(buffer_id, token);
    }
    else
        type = INVALID;
    return type;
}

void put_token_type(char *const token, token_type type)
{
    char string[26] = {0};

    switch (type)
    {
    case WHITESPACE:
        return;
    case KEYWORD:
        strcpy(string, "Keyword");
        break;
    case FUNCTION:
        strcpy(string, "Function call");
        break;
    case ARITH_OP:
        strcpy(string, "Arithmetic Operator");
        break;
    case LOGIC_OP:
        strcpy(string, "Logical Operator");
        break;
    case BIT_OP:
        strcpy(string, "Bitwise Operator");
        break;
    case REL_OP:
        strcpy(string, "Relational Operator");
        break;
    case ARTITH_ASSIGN_OP:
        strcpy(string, "Arith Assign Operator");
        break;
    case IDENTIFIER:
        strcpy(string, "Identifier");
        break;
    case INT_CONST:
        strcpy(string, "Integer Constant");
        break;
    case FLOAT_CONST:
        strcpy(string, "Float Constant");
        break;
    case CHAR_CONST:
        strcpy(string, "Char Constant");
        break;
    case ASSIGN_OP:
        strcpy(string, "Assignment Operator");
        break;
    case SPECIAL:
        strcpy(string, "Special Character");
        break;
    default:
        strcpy(string, "Invalid Character");
    }
    printf("  | %25s | %-25s |\n", token, string);
}

void process_line(char *const line)
{
    char *start = line;
    char *tmp;
    char token[128];
    int i = 0, idx = 0;
    while (line[idx])
    {
        if (!is_delimiter(line[idx]))
        {
            token[i++] = line[idx++];
            if (line[idx] == '\"')
            {
                while (line[idx] && line[idx] != '\"')
                    token[i++] = line[idx++];
                token[i++] = '\"';
                idx++;
            }
            else if (line[idx] == '(')
            {
                while (line[idx] && line[idx] != ')')
                    token[i++] = line[idx++];
                token[i++] = ')';
                idx++;
            }
        }
        else
        {
            token[i] = 0;
            // printf("token is: %s\n", token);
            if (token[0])
                put_token_type(token, identify_token(token));
            i = 0;
            if (!(line[idx] == ' ' || line[idx] == '\n' || line[idx] == '\t'))
            {
                char temp_str[2] = "";
                temp_str[0] = line[idx];
                temp_str[1] = 0;
                printf("  | %25s | %-25s |\n", temp_str, "Special Character");
            }
            idx++;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid Usage! Specify file to parse as argument");
        exit(1);
    }

    char *file_path = argv[1];
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
    {
        perror("File not found: ");
        exit(1);
    }

    int count;
    char buffer[1024] = {0};
    bzero(buffer, sizeof(buffer));

    printf("  +---------------------------+---------------------------+\n");
    printf("  |          TOKEN            |          TOKEN TYPE       |\n");
    printf("  +---------------------------+---------------------------+\n");

    while (fgets(buffer, sizeof(buffer), fp))
    {
        remove_newline(buffer);

        if (buffer[0] == '/' && buffer[1] == '/')
        { //Line comment
            remove_newline(buffer);
            printf("  | %25s | %-25s |\n", buffer, "Single Line comment");
        }
        else if (buffer[0] == '/' && buffer[1] == '*') /* handle block comments*/
        {
            remove_newline(buffer);
            printf("  | %25s | %-25s |\n", buffer, " ");
            bzero(buffer, sizeof(buffer));

            int end_block_found = 0;

            while (fgets(buffer, sizeof(buffer), fp) && !end_block_found)
            {

                if (buffer[0] == '*' && buffer[1] == '/')
                    end_block_found = 1;
                remove_newline(buffer);

                printf("  | %25s | %-25s |\n", buffer, ((end_block_found) ? "Multiline Comment" : " "));
                bzero(buffer, sizeof(buffer));
            }
        }
        else if (check_pre_processor(buffer))
        {
            printf("  | %25s | %-25s |\n", buffer, "Preprocessor Directive");
        }
        else
            process_line(buffer);
        bzero(buffer, sizeof(buffer));
    }
    printf("  +---------------------------+---------------------------+\n");

    distab(symbol_table, symtab_index);
}