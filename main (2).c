#include "header.h"

int main(int argc, char**argv)
{
    int i = 0;
    pid_t pid;
    int flag;
    int n;
    int re_flag;
    int pp_cnt;
    int m;
 
    while(1)
    {
        struct sigaction act;
        act.sa_handler = z_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_RESTART;
        sigfillset(&(act.sa_mask));
        sigaction(SIGCHLD, &act, NULL);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);

        cmdvector[0] = (char*)0;
        flag = 0;
        pp_cnt = 0;
        re_flag = 0;
        fputs(prompt, stdout);
        fgets(cmdline, BUFSIZE, stdin);
        cmdline[strlen(cmdline) -1] = '\0';
        strcpy(cmdline2, cmdline);
        n = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

        m = makelist(cmdline2, "|", PipeVector, MAX_CMD_ARG);
        
        if(cmdvector[0] == (char*)0)
            continue;
        if(!strcmp(cmdvector[n-1], "&"))
        {
            flag = 1;
            cmdvector[n-1] = (char*)0;
        }

        if(!strcmp(cmdvector[0], "cd"))
        {
            chdir(cmdvector[1]);
        }

        else if(!strcmp(cmdvector[0], "exit"))
        {
            exit(1);
        }
        
        else
        {

           switch(pid=fork())
             {
                  case 0:
                     signal(SIGQUIT, SIG_DFL);
                     signal(SIGINT, SIG_DFL);
                     signal(SIGTSTP, SIG_DFL);
                     pp(m, PipeVector);
       
                     fatal("main()");
                  case -1:
                     fatal("main()");
                  default:
                     if(!flag)
                     {
                         waitpid(pid, NULL, 0);
                     }
           
             }
        }
       
    }
    return 0;
}
