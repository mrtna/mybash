#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Command.h"

#define MAX_SIZE 1024

int main() {
	printf("Bienvenue à \"mybash\". \nEntrez les commandes comme sur votre bash habituel.\n");
	char *myCommand = malloc(MAX_SIZE*sizeof(char));

	while(1) {
		scanf("%s", myCommand);
		printf("Command entered : %s\n", myCommand);
	}
}