#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "chain.h"

#define MAX_SIZE 1024

void handle_signal(int signal) {

	printf("%d\n",signal);

	if( signal== SIGINT) {
		printf ("Signal caught, exiting \n");
		exit(0);
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
		if(strcmp(myCommand, "exit") == 0) {
			exit(0);
		}

		Command c = newCommand(myCommand);
		
		List chain = createChainFromString(myCommand);
		executeChain(chain);

// VERSION 1.0, SEULEMENT 1 COMMANDE
		/*pid_t pid = fork();
		if(pid == 0) {
			fflush(stdin);
			executeCommand(c);
		} else {
			// TODO
			pid = wait(NULL);
		}*/
	}
}