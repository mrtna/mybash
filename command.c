#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int addName(Command c, char* name) {
	if(c == NULL || name == NULL) {
		return 0;
	}
	strcpy(c->command_name, name);
	return 1;
}

int addParam(Command c, char* param) {
	if(c == NULL || param == NULL) {
		return 0;
	}
	return 1;
}

char* trim(char *input) {
	// savoir si on se trouve
	// dans une chaine d'espaces
	int numOfSpaces = 0;
	int i = 0;
	int j = 0;
	char *trimmedString = malloc(sizeof(char) * strlen(input));

	// Ignorer les premiers espaces inutiles
	while(input[i] == ' ') {
		i++;
	}

	for(i; input[i] != '\0' ; i++) {
		if(input[i] == ' ') {
			numOfSpaces++;
		} else {
			numOfSpaces = 0;
		}
		if(numOfSpaces <= 1) {
			trimmedString[j] = input[i];
			j++;
		}
	}
	trimmedString[j] = '\0';
	return trimmedString;
}

int parseCommand(char *input, Command c) {
	if(input == NULL) return -1;

	input = trim(input);

	char field_separator = ' ';
	int i = 0;
	int copieur = 0;
	int didPrintEndOfString = 0;
	int count = 0;
	int inputRedirection = 0;
	int outputRedirection = 0;
	int writingFile = 0;
	int nameOver = 0;
	for(i ; input[i] != '\0' ; i++) {
		if(input[i] == field_separator) {
			if(nameOver == 0) {
				c->command_name[copieur] = '\0';
				nameOver = 1;
				count++;
			} else {
				if(inputRedirection) {
					if(writingFile) {
						c->fileRedirectInput[copieur] = '\0';
						writingFile = 0;
						inputRedirection = 0;
					} else {
						writingFile = 1;
					}
				} else if (outputRedirection) {
					if(writingFile) {
						c->fileRedirectOutput[copieur] = '\0';
						writingFile = 0;
						inputRedirection = 0;
					} else {
						writingFile = 1;
					}
				} else {
					if(count == 0) {
						count++;
					}
					c->command_params[count-1][copieur] = '\0';
					c->command_params = realloc(c->command_params, (count) * sizeof(char*));
					c->command_params[count] = malloc(sizeof(char)*32);	
					count++;
				}
			}
			didPrintEndOfString = 1;
			copieur = 0;
		} else {
			didPrintEndOfString = 0;
			if(count == 0 && !writingFile) {
				c->command_name[copieur] = input[i];
			}
			else {
				if(input[i] == '<') {
					count--;
					inputRedirection = 1;
					if(c->redirectionType == OUTPUTEND) {
						c->redirectionType = BOTHEND;
					} else if(c->redirectionType == OUTPUT) {
						c->redirectionType = BOTH;
					} else {
						c->redirectionType = INPUT;
					}
				} else if(input[i] == '>') {
					if(strlen(input) > i+1 && input[i+1] == '>') {
						count--;
						i++;
						outputRedirection = 1;
						if(c->redirectionType == INPUT) {
							c->redirectionType = BOTHEND;
						} else {
							c->redirectionType = OUTPUTEND;
						}
					} else {
						count--;
						outputRedirection = 1;
						if(c->redirectionType == INPUT) {
							c->redirectionType = BOTH;
						} else {
							c->redirectionType = OUTPUT;
						}
					}
				} else {
					if(writingFile) {
						if(inputRedirection) {
							c->fileRedirectInput[copieur] = input[i];
						} else if (outputRedirection) {
							c->fileRedirectOutput[copieur] = input[i];
						}
					} else {
						c->command_params[count-1][copieur] = input[i];
					}
				}
			}
			copieur++;
		}
	}
	if(input[i-1] == field_separator) {
		if(didPrintEndOfString) {
			count--;
		}
	}
	if(didPrintEndOfString == 0) {
			if(count == 0) {
				c->command_name[copieur] = '\0';
			} else {
				c->command_params[count-1][copieur] = '\0';
			}
	}
	c->param_number = count;
	return count;
}

int printCommand(Command c) {
	if(c == NULL) return -1;
	int i;
	printf("Commande : %s\n", c->command_name);
	printf("Param number : %d\n", c->param_number);
	printf("Parametres :\n");
	for(i=0 ; i < c->param_number; i++){
		char* currentString = c->command_params[i];
		int j;
		printf("%s\n", currentString);
	}
}

char **getParamsAsArray(Command c) {
	int i = 0;
	char **params = malloc(2+c->param_number*sizeof(char*));
	params[i] = c->command_name;
	for(i;i<c->param_number;i++) {
		params[i+1] = c->command_params[i];
	}
	params[i+1] = NULL;
	return params;
}

int redirect_cd(char *directory)
{
	int fd = open("/tmp/cdout", O_WRONLY|O_NONBLOCK, 0666);
	if(fd == -1) {
		perror("Cannot open fifo");
		return EXIT_FAILURE;
	}
	if(directory == NULL) {
		directory = "NOTADIRAMNK";
	}
	write(fd, directory, strlen(directory));
	
	close(fd);
	
	kill(getppid(), SIGUSR1);
}

int handle_pwd(char *directory) {
	char currentPwd[1024];
	getcwd(currentPwd, 1024);
	printf("%s\n", currentPwd);
}

int handle_quit(char *param) {
	kill(getppid(), SIGINT);
	exit(0);
}

//fonction permetant de "reconnaitre" la commande cd
char *builtin_str[] = {
	"cd",
	"pwd",
	"exit",
	"quit"
};

int (*builtin_func[]) (char *) = {
	&redirect_cd,
	&handle_pwd,
	&handle_quit,
	&handle_quit
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

int executeCommand(Command c, int *input_fd, int* pipe) {
	int i, found = 0;;
	char **params = getParamsAsArray(c);
	for(i = 0; i< num_builtins(); i++){
		if(strcmp(c->command_name, builtin_str[i]) == 0) {
			found = 1;
			(*builtin_func[i])(params[1]);
		}
	}
	if(!found) {
		int newInput;
		int newOutput;
		switch(c->redirectionType) {
			case NONE :
				newInput = *input_fd;
				newOutput = pipe[1];
			break;
			case INPUT :
				newInput = open(c->fileRedirectInput, O_RDONLY|O_CREAT, 0666);
				newOutput = pipe[1];
			break;
			case BOTH :
				newInput = open(c->fileRedirectInput, O_RDONLY|O_CREAT, 0666);
				newOutput = open(c->fileRedirectOutput, O_RDWR|O_CREAT, 0666);
			break;
			case OUTPUTEND :
				newInput = *input_fd;
				newOutput = open(c->fileRedirectOutput, O_RDWR|O_CREAT, 0666);
				lseek(newOutput, 0, SEEK_END);
			break;
			case OUTPUT :
				newInput = *input_fd;
				newOutput = open(c->fileRedirectOutput, O_RDWR|O_CREAT, 0666);
			break;
			case BOTHEND :
				newInput = open(c->fileRedirectInput, O_RDONLY|O_CREAT, 0666);
				newOutput = open(c->fileRedirectOutput, O_RDWR|O_CREAT, 0666);
				lseek(newOutput, 0, SEEK_END);
			break;
		}
		dup2(newInput, 0);
		dup2(newOutput, 1);
		close(newInput);
		close(newOutput);
		close(pipe[0]);
		close(pipe[1]);
		execvp(params[0], params);
	}
	return 0;
}

Command newCommand(char *input) {
	if(input == NULL) return NULL;
	Command c = (Command) malloc(sizeof(struct COMMAND));
	c->command_params = malloc(sizeof(char*));
	c->command_params[0] = malloc(sizeof(char) * 32);
	c->hasRedirect = 0;
	c->fileRedirectOutput = malloc(256*sizeof(char));
	c->fileRedirectInput = malloc(256*sizeof(char));
	c->redirectionType = NONE;
	parseCommand(input, c);
	return c;
}