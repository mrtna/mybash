#include <stdio.h>
#include <stdlib.h>

typedef struct COMMAND {
	char command_name[128];
	char* command_params[16];
} *Command;

int newCommand();
int addName(Command c, char* name);
int addParam(Command c, char* param);