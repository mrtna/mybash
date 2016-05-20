#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

typedef struct LIST {
	Command c;
	char *fifo;
	Elem next;
	int index;
	char separator;
} *List, *Elem;

List createList();
int count(List l);
int addCommand(List l, Command c);
int executeChain(List l);
List createChainFromString(char *string);