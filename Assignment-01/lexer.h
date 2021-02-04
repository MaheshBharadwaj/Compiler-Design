#define MAX 128

typedef enum token_type
{
    INVALID = 0,
    KEYWORD,
    IDENTIFIER,
    COMMENT,
    INT_CONST,
    FLOAT_CONST,
    CHAR_CONST,
    ARITH_OP,
    ARTITH_ASSIGN_OP,
    LOGIC_OP,
    REL_OP,
    BIT_OP,
    UNARY_OP,
    ASSIGN_OP,
    SPECIAL,
    FUNCTION,
    PRE_PROCESSOR,
    WHITESPACE
} token_type;

const char keywords[MAX][30] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "\0"};

const char arith_op[MAX][30] = {"+", "-", "*", "/", "%", "\0"};

const char arith_assign_op[MAX][30] = {"+=", "-=", "*=", "/=", "%=", "\0"};

const char logic_op[MAX][30] = {"&&", "||", "!", "\0"};

const char rel_op[MAX][30] = {"<", "<=", ">", ">=", "==", "!=", "\0"};

const char bit_op[MAX][30] = {"^", "&", "|", "<<", ">>", "\0"};

const char unary_op[MAX][30] = {"-", "++", "--", "\0"};

const char assign_op[MAX][30] = {"=", "\0"};

const char special[MAX][30] = {";", ".", "[", "]", "(", ")", "{", "}", "[", "]", ",", "\0"};

const char data_type[MAX][30] = {"char", "int", "float", "double", "\0"};

typedef struct entry
{
    int sno;
    char id[30];
    char type[30];
    int addr;
    char val[30];
} entry;

entry symbol_table[MAX] = {0};

int check_identifier_table(char *const id, entry symbol_table[])
{
    int found = 0;
    for (int i = 0; i < MAX && symbol_table[i].sno; i++)
    {
        if (strcmp(symbol_table[i].id, id) == 0)
        {
            found = 1;
            break;
        }
    }
    return found;
}

void distab(entry s[], int len)
{
    printf("\n+----------------+----------------+----------------+----------------+");
    printf("\n\tName\t\tType\t\tAddress\t\t Value");
    printf("\n+----------------+----------------+----------------+----------------+\n");
    for (int i = 0; i < len; i++)
    {
        printf("\t%s\t\t%s\t\t%d\t\t%s\n", s[i].id, s[i].type, s[i].addr, s[i].val);
    }
}

int check(const char *const token, const char array[][30])
{
    int found = 0;

    for (int i = 0; i < MAX && array[i][0]; i++)
    {
        if (strcmp(token, array[i]) == 0)
        {
            found = 1;
            break;
        }
    }

    return found;
}

int check_real_const(const char *token)
{
    int count = 0, digit_found = 0;
    int i = (token[0] == '-' || token[0] == '+') ? 1 : 0;
    for (; token[i]; i++)
    {
        if (isdigit(token[i]))
        {
            digit_found = 1;
            continue;
        }
        if (token[i] == '.')
            count = 1;
        else
            return -1;
    }
    if (count > 1 || !digit_found)
        return -1;
    return count;
}

int check_char_const(const char *token)
{
    if (token[0] != '\"')
        return 0;
    int count = 0;
    for (int i = 0; token[i]; i++)
        if (token[i] == '\"')
            count++;
    return (count == 2);
}

int check_indentifier(const char *token)
{
    if (isdigit(token[0]))
        return 0;
    for (int i = 0; token[i]; i++)
    {
        if (!(isalnum(token[i]) || token[i] == '_'))
            return 0;
    }
    return 1;
}

int check_pre_processor(const char *line)
{
    if (line[0] == '#')
        return 1;
    return 0;
}

int check_function(const char *token)
{
    char test[128] = {0};
    int idx = 0, i = 0, found = 0;
    while (token[idx])
    {
        if (token[idx] != '(')
        {
            test[i++] = token[idx++];
            found = 1;
        }
        else
        {
            test[i] = 0;
            break;
        }
    }
    if (found == 1)
    {
        while (token[idx])
        {
            if (token[idx] == ')')
            {
                found = 2;
                break;
            }
            idx++;
        }
        if (check_indentifier(test) && found == 2)
            return 1;
    }
    return 0;
}

int is_delimiter(const char c)
{
    for (int i = 0; special[i][0]; i++)
        if (special[i][0] == '.')
            continue;
        else if (c == special[i][0])
            return 1;
        else
            ;
    if (c == ' ' || c == '\n' || c == '\t')
        return 1;
    return 0;
}
