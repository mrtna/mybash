#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

typedef enum { BOTH, BOTHEND, INPUT, OUTPUT, OUTPUTEND, NONE } RedirectionType;

typedef struct COMMAND {
	char command_name[128];
	char **command_params;
	int param_number;

	int hasRedirect;
	RedirectionType redirectionType;
  	char *fileRedirectOutput;
  	char *fileRedirectInput;
} *Command;

Command newCommand(char* input);

int addName(Command c, char* name);

int addParam(Command c, char* param);

int countParams(char *input);

int printCommand(Command c);

int executeCommand(Command c, int *fdin, int* pipe);