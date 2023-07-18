#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>



int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
    {
        i++;
    }
    return i;
}

void ft_error(char *msg, char *cmd)
{
    write(2, msg, ft_strlen(msg));
    write(2, " ", 1);
    write(2, cmd, ft_strlen(cmd));
    write(2, "\n", 1);
}

void ft_build(char *cmd)
{
    int a = 0;
    if(cmd == NULL)
    {
        write(2, "error: cd: bad arguments\n",25);
        return;
    }
    a = chdir(cmd);
    if(a == - 1){
       ft_error("error: cd: cannot change directory to ", cmd); 
    }
}

void ft_execute(char ***cmds, char **args, char **env)
{

    int i = 0;
    int fds[2];
    int id = 0;
    int ret = 0;
    i = 0;
    while(cmds[i])
    {
        if(cmds[i][0] == NULL)
            break;
        if(args[i])
            pipe(fds);
        if(cmds[i][0] && strcmp(cmds[i][0], "cd") == 0)
            {
                if(i == 0 && strcmp(args[i], "|") != 0)
                    ft_build(cmds[i][1]);
                else if(i != 0 && strcmp(args[i], "|") != 0 && strcmp(args[i - 1], "|") != 0)
                     ft_build(cmds[i][1]);
            }
        else
        {

        id = fork();
        if(id < 0)
            exit(0);
        else if(id == 0)
        {
            if(args[i] != NULL)
            {
                if(strcmp(args[i],"|") == 0)
                    dup2(fds[1], 1);
            }
            
            ret = execve(cmds[i][0], cmds[i],env);
            ft_error("error: cannot execute", cmds[i][0]);
            exit(0);
        }
        dup2(fds[0], 0);
        close(fds[0]);
        close(fds[1]);
        if(args[i] && strcmp(args[i], ";") == 0)
        {
            while(waitpid(0, NULL, 0) != -1); 
        }
        }
        i++;
    }
    while(waitpid(0, NULL, 0) != -1); 
    exit(0);
    }
   

int main(int argc, char **argv, char **env)
{
    char ***cmds = NULL;
    char **args = NULL;
    int i = 1;
    int j = 0;
    int k = 0;
    int b = 0;
    if(argc < 2)
        return 0;
    else
    {
        while(argv[i])
        {
            if(strcmp(argv[i],";") == 0 || strcmp(argv[i], "|") == 0)  
                j++;          
            i++;
        }
        cmds = calloc(sizeof(char **) , (j + 2));
        args = calloc(sizeof(char *) , (j + 1));
        i = 1;
        while(argv[i])
        {
            j = 0;
            while(argv[i] && (strcmp(argv[i],";") != 0 && strcmp(argv[i], "|") != 0))
            {
                j++;
                i++;
            }
            cmds[k] = calloc(sizeof(char *) , j + 1);
            if(j != 0 && argv[i])
                    k++;
            if(argv[i])
            {
                i++;  
            }
        }
        i = 1;
        k = 0;
        while(argv[i])
        {
           j = 0;
            while(argv[i] && (strcmp(argv[i],";") != 0 && strcmp(argv[i], "|") != 0))
            {
                cmds[b][j]  = argv[i];
                j++;
                i++;
            }
            if(j != 0)
                b++;
            if(argv[i])
            {
                args[k] = argv[i];
                i++;
                k++;       
            }
        }
    }
ft_execute(cmds,args, env);
}