typedef struct
{
    char left[10];
    char right[128];
} Production;

Production *eliminate_left_recursion(Production, int *);
void getProduction(Production *);
void putProduction(const Production);

void getProduction(Production *p)
{
    bzero(p, sizeof(Production));
    scanf("%s --> %s", p->left, p->right);
}

void putProduction(const Production p)
{
    printf("%s --> %s\n", p.left, p.right);
}

Production *eliminate_left_recursion(Production p, int *count)
{
    Production *p_list = calloc(2, sizeof(Production));
    strcpy(p_list[0].left, p.left);

    char buffer[128], p_right_copy[128];
    strcpy(p_right_copy, p.right);
    char *token = strtok(p_right_copy, "|");
    int lr_detected = 0;
    while (token != NULL)
    {
        if (token[0] == p.left[0])
        {
            lr_detected = 1;
            break;
        }
        token = strtok(NULL, "|");
    }

    if (!lr_detected)
    {
        strcpy(p_list[0].left, p.left);
        strcpy(p_list[0].right, p.right);
        (*count) = 1;
        return p_list;
    }
    strcat(p_list[1].left, p.left);
    strcat(p_list[1].left, "'");
    strcpy(p_right_copy, p.right);
    token = strtok(p_right_copy, "|");
    while (token != NULL)
    {
        if (token[0] == p.left[0])
        {
            lr_detected = 1;
            strcat(p_list[1].right, &token[1]);
            buffer[0] = p.left[0];
            buffer[1] = '\'';
            buffer[2] = '|';
            buffer[3] = 0;
            strcat(p_list[1].right, buffer);
        }
        else
        {
            strcat(p_list[0].right, token);
            buffer[0] = p.left[0];
            buffer[1] = '\'';
            buffer[2] = '|';
            buffer[3] = 0;
            strcat(p_list[0].right, buffer);
        }
        token = strtok(NULL, "|");
    }
    p_list[0].right[strlen(p_list[0].right) - 1] = 0;
    strcat(p_list[1].right, "epsilon");
    (*count) = 2;
    return p_list;
}