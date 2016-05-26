#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "command.h"

typedef enum { PIPE, IFFAIL, IFSUCCESS } ChainType;

typedef struct LIST LIST, *List;

struct LIST {
	Command c;
	char *fifo;
	List next;
	int index;
	ChainType type;
  	int endsWithFile;
};

List createList();
int count(List l);
int addCommand(List l, Command c);
int executeChain(List l, char *currentFolder);
List createChainFromString(char *string);
