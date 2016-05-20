#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "chain.h"
#include "command.h"

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
	while(current->c != NULL) {
		if(current->next != NULL) 
			current = current->next;
		i++;
	}
}

int addCommand(List l, Command c) {
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
	do {
      pipe(p);
      if ((pid = fork()) == -1)
        {
          exit(EXIT_FAILURE);
        }
      else if (pid == 0)
        {
          dup2(fd_in, 0); //change the input according to the old one 
          if (*(cmd + 1) != NULL)
            dup2(p[1], 1);
          close(p[0]);
          execvp((*cmd)[0], *cmd);
          exit(EXIT_FAILURE);
        }
      else
        {
          wait(NULL);
          close(p[1]);
          fd_in = p[0]; //save the input for the next command
          cmd++;
        }
		executeCommand(current->c);
	} while(current != NULL);
}

List createChainFromString(char *string) {
	List list = createList();
	int i = 0;
	int j = 0;
	char * current = malloc(1024*sizeof(char));
	List currentElem = list;
	for(i ; i < strlen(string) ; i++) {
		if(string[i] == '|') {
			if(strlen(string) - i > 1) {
				if(string[i+1] == ' ') {
					currentElem->c = newCommand(current);
					i++;
				} else if(string[i+1] == '|') {

				} else {
					printf("Erreur de format\n");
				}
			} else {
				printf("Erreur de format");
			}
		} else {
			current[j] = string[i];
			j++;
		}
	}
	currentElem->c = newCommand(current);
	return list;
}