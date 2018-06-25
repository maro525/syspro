/*
シェルを作るぷるグラム

pipeはできてない。。。
cdも対応してはい。。。
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *read_command(void)
{
    int bufsize = 1024;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    while(1)
    {
        c = getchar();

        if(c == EOF)
        {
            exit(EXIT_SUCCESS);
        }
        else if (c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        if(position >= bufsize)
        {
            bufsize += bufsize;
            buffer  =  realloc(buffer, bufsize);
        }
    }
}

char **split_command(char *line)
{
    int bufsize = 64, position = 0;
    char **args = malloc(bufsize * sizeof(char*));
    char *token, **args_backup;

    token = strtok(line, " ");
    while(token != NULL)
    {
        args[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += bufsize;
            args_backup = args;
            args = realloc(args, bufsize * sizeof(char*));
            
            if(!args) free(args_backup);
        }

        token = strtok(NULL, " ");
    }
    args[position] = NULL;
    return args;
}

int command_execute(char **args)
{
    int status;

    if(args[0] == NULL)
        return 1;

    pid_t pid = fork();

    if(pid == 0)
    {
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        perror("command_execute");
    }
    else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char *comm;
    char **args;
    int status;

    do {
        printf("\nInput command ...\n");
        printf("-> ");  
        comm = read_command();
        args = split_command(comm);
        status = command_execute(args);

        free(comm);
        free(args);
    } while(status);

    return 0;
}