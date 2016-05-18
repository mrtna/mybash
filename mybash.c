#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "command.h"
#include <unistd.h>

#define MAX_SIZE 1024

int main() {
	printf("Bienvenue à \"mybash\". \nEntrez les commandes comme sur votre bash habituel.\n");
	char *myCommand = malloc(MAX_SIZE*sizeof(char));
	execl("ls", "ls", "-l", NULL);

	while(1) {
		printf("> ");
		fflush(stdout);
		scanf("%[^\n]%*c", myCommand);
		Command c = newCommand(myCommand);
		printCommand(c);
		char *string_params = getStringFromParams(c);
		printf("Hey : %s\n", string_params);
		execl(c->command_name, c->command_name, string_params, NULL);
	}
}