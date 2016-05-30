#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "chain.h"

List createList() {
	List list = malloc(sizeof(struct LIST));
	list->next = NULL;
	list->c = NULL;
	list->fifo = NULL;
	list->index = 0;
	return list;
}

int count(List l) {
	if(l == NULL) return 0;
	int i = 0;
	List current = l;
	while(current != NULL && current->c != NULL) {
		current = current->next;
		i++;
	}
	return i;
}

int addCommandToList(List l, Command c) {
	if(l == NULL) return -1;
	if(c == NULL) return -1;
	if(l->c == NULL) {
		l->c = c;
	} else {
		List current = l;
		while(l->next != NULL) {
			current = l->next;
		}
		current->next = malloc(sizeof(struct LIST));
		current->next->c = c;
		current->next->index = count(l);
	}
	return 1;
}

int executeChain(List l) {
	if(l == NULL) return -1;
	if(l->c == NULL) return -1;
	List current = l;
	int p[2];
	pid_t pid;
	int fd_in = 0;
	int i = 0;
	do {
		pipe(p);
		if ((pid = fork()) == -1){
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
			int v = executeCommand(current->c, &fd_in, p);
			exit(v);
		} else {
			int status;
			close(p[1]);	
			wait(&status);		

			if(status == 0) {
				if(current->index == count(l)-1) {
					char *readbuffer = malloc(1*sizeof(char));
					int nbytes;
					while(nbytes = read(p[0], readbuffer, sizeof(readbuffer)) > 0){
	                	printf("%s", readbuffer);
	        		}

				} else {
					fd_in = p[0]; //save the input for the next command
				}
				if(current->type == IFFAIL) {
					current = current->next;
					if(current == NULL) {
						printf("Erreur dans la syntaxe\n");
						return -1;
					} else {
						current = current->next;
					}
				} else if(current->type == IFSUCCESS) {
					current = current->next;
				} else {
					current = current->next;
				}    	
			} else {
				close(p[0]);
				return -1;
			}
		}
		i++;
	} while(current != NULL);
	close(p[0]);
}

List chainCommand(List currentElem, char *string, ChainType type) {
    currentElem->c = newCommand(string);
    currentElem->next = malloc(sizeof(struct LIST));
    currentElem->next->index = currentElem->index+1;
    currentElem->type = IFSUCCESS;
    currentElem = currentElem->next;
    return currentElem;
}

List createChainFromString(char *string) {
	List list = createList();
	int i = 0;
	int j = 0;
	char * current = malloc(1024*sizeof(char));
	List currentElem = list;
	for(i ; i < strlen(string) ; i++) {

    // On regarde si on
    // souhaite effectuer un "sinon" (avec ||)
    // ou un pipe (avec |)
		if(string[i] == '|') {
      // TODO gérer le type.
			if(strlen(string) - i > 1) {
				if(string[i+1] == ' ') {
					currentElem = chainCommand(currentElem, current, PIPE);
					i++;
					j=0;
					current = malloc(1024*sizeof(char));
				} else if(string[i+1] == '|') {
					currentElem = chainCommand(currentElem, current, IFFAIL);
			          i++;
			          j=0;
				} else {
					printf("Erreur de format\n");
				}
			} else {
				printf("Erreur de format");
			}
		} else if(string[i] == '&' && strlen(string) - i > 1 && string[i+1] == '&') {
			currentElem = chainCommand(currentElem, current, IFSUCCESS);
          i++;
          j=0;
		}
    // Sinon, on ajoute simplement
    // la chaine parcourue
    // à la chaine courante.
    else {
			current[j] = string[i];
			j++;
		}
	}
	currentElem->c = newCommand(current);
	return list;
}
