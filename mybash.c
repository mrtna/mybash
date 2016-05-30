#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "chain.h"

#define MAX_SIZE 1024

char currentFolder[MAX_SIZE]; 
char oldFolder[MAX_SIZE]; 
int fd;

void createFifo() {
	int codefifo = mkfifo("/tmp/cdout", 0666);
	fd = open("/tmp/cdout", O_RDONLY|O_NONBLOCK);
	if (fd == -1) {
		perror("Cannot open FIFO for read\n");
	}
}

int handle_cd() {
	char string[MAX_SIZE];
	memset(string, 0, MAX_SIZE);
	
	int res;
	char current;
	int i = 0;
	lseek(fd, 0, SEEK_SET);
	while(1) {
		res = read(fd, &current, 1);
		if(res == 0) {
			string[i] = '\0';
			break;
		}
		string[i] = current;
		i++;
	}
	if(strlen(string) && string[0] == '-') {
		chdir(oldFolder);
		strcpy(oldFolder, currentFolder);
		getcwd(currentFolder, sizeof(currentFolder));
	} else if (strlen(string) && string[0] == '~'){
		chdir(getenv("HOME"));
		strcpy(oldFolder, currentFolder);
		getcwd(currentFolder, sizeof(currentFolder));
	}
	else {
		if(chdir(string) != 0) {
			perror("custom_cd");
		} else {
			strcpy(oldFolder, currentFolder);
			getcwd(currentFolder, sizeof(currentFolder));
		}
	}
}

void handle_signal(int signal) {
	if( signal== SIGINT) {
		printf("\n\n=====================\nBye !\nDevelopped by KNORR Nicolas and MARTIN Anthony\n\n");
		close(fd);
		exit(0);
	} else if (signal == SIGUSR1) {
		handle_cd();
	}
}


int main() {

	createFifo();

	getcwd(currentFolder, sizeof(currentFolder));

	struct sigaction sa;

    sa.sa_handler = &handle_signal;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

	//printf("Bienvenue à \"mybash\". \nEntrez les commandes comme sur votre bash habituel.\n");
	char *myCommand = malloc(MAX_SIZE*sizeof(char));
	while(1) {
		int i = 0;
		printf("%s> ", currentFolder);
		fflush(stdout);
		fgets(myCommand, MAX_SIZE, stdin);
		myCommand[strlen(myCommand)-1] = '\0';

		List chain = createChainFromString(myCommand);
		executeChain(chain);
	}
}
