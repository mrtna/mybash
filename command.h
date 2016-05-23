#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct COMMAND {
	char command_name[128];
	char **command_params;
	int hasRedirect;
  	char *fileRedirect;
	int param_number;
	pid_t parentPid;
} *Command;

Command newCommand(char* input);

int addName(Command c, char* name);

int addParam(Command c, char* param);

int countParams(char *input);

int printCommand(Command c);

void executeCommand(Command c);