#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>
#define MAX_CMD_ARG 20
#define BUFSIZE 256

static char *prompt = "mysh> ";
char* cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZE];
char *PipeConVec[MAX_CMD_ARG];
char *PipeVector[MAX_CMD_ARG];
char cmdline2[BUFSIZE];

void fatal(char *str);
int makelist(char *s, const char *delimiters, char** list, int MAX_LIST);
void z_handler();
void redirection(char **cmd);
void pp(int pp_cnt, char **cmd);
