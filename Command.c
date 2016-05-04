#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h"

Command newCommand() {
	Command c = (Command) malloc(sizeof(struct COMMAND));
}

int addName(Command c, char* name) {
	if(c == NULL || name == NULL) {
		return 0;
	}
	strcpy(c->command_name, name);
	return 1;
}