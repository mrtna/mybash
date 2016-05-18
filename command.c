#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

Command newCommand(char *input) {
	if(input == NULL) return NULL;
	Command c = (Command) malloc(sizeof(struct COMMAND));
	c->command_params = malloc(sizeof(char*));
	c->command_params[0] = malloc(sizeof(char) * 32);
	parseCommand(input, c);
	return c;
}

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

	printf("String : %s\n", input);

	input = trim(input);

	printf("Trimmed : %s\n", input);

	char field_separator = ' ';
	int i = 0;
	int copieur = 0;
	int didPrintEndOfString = 0;
	int count = 0;
	for(i ; input[i] != '\0' ; i++) {
		if(input[i] == field_separator) {
			if(count == 0) {
				c->command_name[copieur] = '\0';
			} else {
				c->command_params[count-1][copieur] = '\0';
			}
			didPrintEndOfString = 1;
			count++;
			if(count > 1) {
				c->command_params = realloc(c->command_params, (count) * sizeof(char*));
				c->command_params[count-1] = malloc(sizeof(char)*32);
			}
			copieur = 0;
		} else {
			didPrintEndOfString = 0;
			if(count == 0) {
				c->command_name[copieur] = input[i];
			}
			else {
				c->command_params[count-1][copieur] = input[i];
			}
			copieur++;
		}
	}
	if(input[i-1] == field_separator) {
		count--;
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
		printf("%d° - %s\n", i, currentString);
	}
}

char *getStringFromParams(Command c) {
	char *finalString = malloc(sizeof(char)*32*c->param_number);
	int i = 0;
	int j = 0;
	int k = 0;
	for(i ; i < c->param_number ; i++) {
		for(j ; j < strlen(c->command_params[i]) ; j++) {
			finalString[k] = c->command_params[i][j];
			k++;
		}
		finalString[k] = ' ';
		k++;
	}
	finalString[k] = '\0';
	return finalString;
}