#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "command.h"
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#define MAX_SIZE 1024

void handle_signal(int signal) {

	printf("%d\n",signal);

	if( signal== SIGINT) {
		printf ("Signal caught, exiting \n");
		kill(getpid(), SIGKILL);
	}
}

int main() {

	struct sigaction sa;

    sa.sa_handler = &handle_signal;

    sigaction(SIGINT, &sa, NULL);

	printf("Bienvenue à \"mybash\". \nEntrez les commandes comme sur votre bash habituel.\n");
	char *myCommand = malloc(MAX_SIZE*sizeof(char));
	while(1) {
		printf("> ");
		fflush(stdout);

		scanf("%[^\n]%*c", myCommand);

		Command c = newCommand(myCommand);
		pid_t pid = fork();
		if(pid == 0) {
			fflush(stdin);
			executeCommand(c);
		} else {
			wait(pid);
		}
	}
}