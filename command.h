#include <stdio.h>
#include <stdlib.h>

typedef struct COMMAND {
	char command_name[128];
	char **command_params;
	int param_number;
} *Command;

Command newCommand(char* input);

int addName(Command c, char* name);

int addParam(Command c, char* param);

int countParams(char *input);

int printCommand(Command c);

void executeCommand(Command c);