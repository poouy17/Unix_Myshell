#include "header.h"

void fatal(char *str)
{
    perror(str);
    exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST)
{
    int i = 0;
    int numtokens = 0;
    char *snew = NULL;

    if( (s==NULL) || (delimiters==NULL) ) return -1;

    snew = s+ strspn(s, delimiters);
    if( (list[numtokens]=strtok(snew, delimiters)) == NULL)
        return numtokens;
    numtokens = 1;

    while(1)
    {
        if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
            break;
        if(numtokens == (MAX_LIST-1)) return -1;
        numtokens++;
    }
    return numtokens;
}

void z_handler()
{
    pid_t child;
    int state;
    waitpid(-1, &state, WNOHANG);
}

void redirection(char **cmd)
{
    int i;
    int fd;
    for (i = 0; cmd[i] != NULL; i++)
    {
        if(!strcmp(cmd[i], ">"))
        {
           fd = open(cmd[i+1], O_RDWR | O_CREAT, 0644);
           dup2(fd, 1);
           close(fd);
           for(i = i +2; cmd[i] != NULL; i++)
           {
               cmd[i-2] = cmd[i];
           }
           cmd[i-2] = NULL;
        }

    }
    
    for(i = 0; cmd[i] != NULL; i++)
    {
        if(!strcmp(cmd[i], "<"))
        {
            fd = open(cmd[i+1], O_RDWR);
            dup2(fd, 0);
            close(fd);
            for(i = i; cmd[i] != NULL; i++)
            {
                cmd[i] = cmd[i+2];
            }

            cmd[i] = NULL;
        }
    }
}


void pp(int m, char ** cmd)
{
    int i;
    int p[2];
    int pp_cnt = m-1;
    int x;
    int y;
    
    for(i = 0; i < pp_cnt; i++)
    {
        pipe(p);
        switch(fork())
        {
            case -1: fatal("error in pp at fork");
            case 0:
                    dup2(p[1], 1);
                    close(p[0]);
                    if(makelist(cmd[i], " \t", PipeConVec, MAX_CMD_ARG) <= 0)
                    {
                        fprintf(stderr, "error in makelist");
                    }
                   // fprintf(stderr, "%s", cmd[i]);
                   // fprintf(stderr, "%s", PipeConVec[0]);
                    redirection(PipeConVec);
                    execvp(PipeConVec[0], PipeConVec);
                    fatal("exec in switch case:0");
            default:
                    dup2(p[0], 0);
                    close(p[1]);
                   //fprintf(stderr, "4");
        }
    }
    makelist(cmd[i], " \t", PipeConVec, MAX_CMD_ARG);
    //fprintf(stderr, "%s", cmd[i]);
    //fprintf(stderr, "%s", PipeConVec[0]);
    redirection(PipeConVec);
    execvp(PipeConVec[0], PipeConVec);
    fatal("exec error in parent");
}




   
