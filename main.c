#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "mysh.h"

int main() 
{
    char cwd[1024];

    while (1) 
    {        
        getcwd(cwd, sizeof(cwd));
        char baseline[sizeof(cwd) + 100];
        snprintf(baseline, sizeof(baseline), "mysh@localhost~/%s: ", cwd);
        fflush(stdout);
        char *input = readline(baseline);
        if (input == NULL) 
        {
            break;
        }
        add_history(input);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) 
        {
            break;
        }

        runcom(input);
        free(input);
    }

    return 0;
}


