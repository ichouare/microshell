#include <unistd.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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


void ft_excute(char ***cmds, char **args, char **env)
{
    int i = 0;
    int id = 0;
    int fd[2];

    while (cmds[i])
    {
      if(args[i])
        pipe(fd);
    if(strcmp(cmds[i][0], "cd") == 0)
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
            exit(40);
        else if(id == 0)
        {
            if(args[i] != NULL)
            {
                if(strcmp(args[i], "|") == 0)
                    dup2(fd[1], 1);
            }
            execve(cmds[i][0], cmds[i], env);
            ft_error("error: cannot execute", cmds[i][0]);
            exit(165);
        }
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        if (args[i] && strcmp(args[i], ";") == 0)
        {
            while(waitpid(0, NULL, 0) != -1); 
        }
    }
        i++;
    }
    while(waitpid(0, NULL, 0) != -1); 
    exit(0);
    }

int main(int argc , char **argv, char **env)
{
    char ***cmds = NULL; 
    char **args = NULL;
    int i = 1;
    int j = 0;
    int k = 0;
    int b = 0;
    if(argc < 2)
        return 0;
   
    while(i < argc)
    {
        if(strcmp(argv[i], ";") == 0 || strcmp(argv[i], "|") == 0)
        {
            j++;
        }
        i++;
    }
    args = calloc(sizeof(char *) , j  + 1);
    cmds = calloc(sizeof(char **) , (j + 2 ));
    if(!args || !cmds)
        exit(100);
    
    i = 1;
    j = 0;
    while(i <  argc )
    {   
        j = 0;
        while(argv[i] && (strcmp(argv[i], ";") != 0 && strcmp(argv[i], "|") != 0))
        {
            j++;
            i++;
        }
        if(j != 0)
        {
            
            cmds[k] = calloc(sizeof(char *) , j + 1);
            if(cmds[k] == NULL)
                exit(12);
            k++;
        } 
        if(argv[i])
            i++;  
    }
    
    i = 1;
    k = 0;
    b = 0;
    while(argv[i])
    {
        j = 0;
        while(argv[i] && (strcmp(argv[i], ";") != 0 &&  strcmp(argv[i], "|") != 0))
        {
            cmds[b][j] = argv[i];
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
ft_excute(cmds, args, env);
}