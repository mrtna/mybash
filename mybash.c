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

	if( signal== SIGINT) {
		printf("Bye !");
		exit(0);
	}
}
//Commande cd
int lsh_cd(char **args)
{
	if(args[1] == NULL) {
		fprintf(stderr, "> Argument attendu pour cd \n");
	} else {
		if(chdir(args[1]) != 0) {
			perror("lsh");
		}
	}
	return 1;
}
//fonction permetant de "reconnaitre" la commande cd
char *builtin_str[] = {
	"cd"
};

int (*builtin_func[]) (char **) = {
	&lsh_cd
};

int lsh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}


//Fonction qui permet d'éxécuter une commande builtin (cf:cd) ou sinon une commande forké (ls etc...)
int lsh_exec(char *params)
{
	 int i,j;
	char **paramz; 
	List chain = createChainFromString(params);
	j=printCommand(chain->c);//cette ligne là n'est que pour le test
	paramz = getParamsAsArray(chain);
	for(i = 0; i< lsh_num_builtins(); i++){
			if(strcmp(paramz[0], builtin_str[i]) == 0) {
				return (*builtin_func[i])(paramz);
			}
		}
		return executeChain(chain);
}
int main() {

	struct sigaction sa;

    sa.sa_handler = &handle_signal;

    sigaction(SIGINT, &sa, NULL);

	printf("Bienvenue à \"mybash\". \nEntrez les commandes comme sur votre bash habituel.\n");
	char *myCommand = malloc(MAX_SIZE*sizeof(char));
	while(1) {
		int i = 0;
		printf("> ");
		fflush(stdout);
		
		scanf("%[^\n]%*c", myCommand);
		i = lsh_exec(myCommand);

		/*List chain = createChainFromString(myCommand);
		executeChain(chain);*/

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
