#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "production.h"

int main()
{
    char buffer[256] = {0};
    int count = 0;
    int opt;
    Production p = {0}, *p_list = NULL;
    do
    {
        printf("Enter the production: ");
        getProduction(&p);

        p_list = eliminate_left_recursion(p, &count);
        for (int i = 0; i < count; i++)
            putProduction(p_list[i]);
        free(p_list);

        printf("Do you want to continue 1/0: ");
        scanf("%d", &opt);
    } while (opt);
}