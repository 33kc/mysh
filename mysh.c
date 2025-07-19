#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mysh.h"

#define COMMAND_HISTORY "/home/kc/mysh/.mysh_history.txt"

void runcom(char *input) 
{
   
    FILE *pCOMMAND_HISTORY = fopen(COMMAND_HISTORY, "a");
    if (pCOMMAND_HISTORY == NULL) 
    {
      perror("Failed to open command history file");
      return;
    }
    fprintf(pCOMMAND_HISTORY, "%s\n", input);
    fclose(pCOMMAND_HISTORY);

    char *args[10];
    int i = 0;
    char *token = strtok(input, " ");
    while (i < 9 && token) 
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    
    static char prev_dir[1024] = "";

    if (strcmp(args[0], "cd") == 0)
    {
        char *target = args[1];
        char curr_dir[1024] = "";

        getcwd(curr_dir, sizeof(curr_dir)); 

        if (target == NULL || strcmp(target, "~") == 0)
        {
            target = getenv("HOME");
        }
        else if (strcmp(target, "-") == 0)
        {
          if (strlen(prev_dir) > 0)
          { 
            target = prev_dir;
            printf("%s\n", target);  
          }
        else
        {
            fprintf(stderr, "mysh: OLDPWD not set\n");
            return;
        }
    }

        if (chdir(target) != 0)
        {
            perror("cd failed");
        }

        strcpy(prev_dir, curr_dir);
        return;
    }


    pid_t pid = fork();

    if (pid == 0) 
    {
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    }
    else if (pid > 0) 
    {
        wait(NULL);
    }
    else
    {
        perror("fork failed");
    }
    
}

