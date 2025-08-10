#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>      
#include <sys/types.h>
#include <sys/stat.h>
#include "mysh.h"

#define COMMAND_HISTORY "/home/kc/Programming/bigprojects/mysh/.mysh_history.txt"

static char prev_dir[1024] = "";

void handle_cd(char **args)
{
    char *target = args[1];
    char curr_dir[1024] = "";

    if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
    {
        perror("getcwd failed");
        return;
    }

    if (target == NULL || strcmp(target, "~") == 0)
    {
        target = getenv("HOME");
        if (!target)
        {
            fprintf(stderr, "mysh: HOME not set\n");
            return;
        }
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
    else
    {
        strcpy(prev_dir, curr_dir);
    }
}

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

    char *args[100];
    int argc = 0;

    char input_copy[1024];
    strncpy(input_copy, input, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';

    char *token = strtok(input_copy, " ");
    while (token != NULL && argc < 99)
    {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    if (argc == 0)
    {
        return; 
    }
    if (strcmp(args[0], "cd") == 0)
    {
        handle_cd(args);
        return;
    }

    int in_redirect_pos = -1, out_redirect_pos = -1;
    char *infile = NULL, *outfile = NULL;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            if (i + 1 < argc)
            {
                infile = args[i + 1];
                in_redirect_pos = i;
                i++;
            }
            else
            {
                fprintf(stderr, "mysh: syntax error near unexpected token `newline'\n");
                return;
            }
        }
        else if (strcmp(args[i], ">") == 0)
        {
            if (i + 1 < argc)
            {
                outfile = args[i + 1];
                out_redirect_pos = i;
                i++; 
            }
            else
            {
                fprintf(stderr, "mysh: syntax error near unexpected token `newline'\n");
                return;
            }
        }
    }

    char *clean_args[100];
    int k = 0;
    for (int i = 0; i < argc; i++)
    {
        if (i == in_redirect_pos || i == in_redirect_pos + 1 ||
            i == out_redirect_pos || i == out_redirect_pos + 1)
        {
            continue;
        }
        clean_args[k++] = args[i];
    }
    clean_args[k] = NULL;

    pid_t pid = fork();

    if (pid == 0)
    {
        if (infile)
        {
            int fd = open(infile, O_RDONLY);
            if (fd < 0)
            {
                perror("open infile");
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2 infile");
                close(fd);
                exit(1);
            }
            close(fd);
        }

        if (outfile)
        {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open outfile");
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2 outfile");
                close(fd);
                exit(1);
            }
            close(fd);
        }

        execvp(clean_args[0], clean_args);
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

