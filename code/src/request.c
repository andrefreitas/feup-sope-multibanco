#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#define MAX_LINE 100
void request_create(struct Request* r, unsigned int pid, char* who,
		char* request) {
	r->pid = pid;
	r->who = who;
	r->request = request;
}

int request_send(struct Request* r, char* fifoname) {
	FILE *fifo = fopen(fifoname, "a+");
	if (fifo) {
		char wrStr[128];

		sprintf(wrStr, "%s: %d > %s\n", r->who, r->pid, r->request);
		fwrite(wrStr, sizeof(char) * strlen(wrStr), 1, fifo);

		return 0;
	}
	return 1;
}

int request_waitAnswer(char* fifoname) {
	FILE *fifo = fopen(fifoname, "r");
	char line[128];

	while (fgets(line, sizeof line, fifo) == NULL) {
	}

	if (strcmp(line,"OK")==0 || strcmp(line,"OK\n")==0 )
		return 1;

	if (strcmp(line,"FAIL")==0 || strcmp(line,"FAIL\n")==0)
		return 0;
	return 0;
}

int request_writeAnswer(char* fifoname, char* answer) {
	FILE *fifo = fopen(fifoname, "a+");
	if (fifo) {
		fwrite(answer, sizeof(char) * strlen(answer), 1, fifo);

		return 0;
	}

	return 1;
}

int request_readAnswer(char* fifoname, char* answer) {
	FILE *fifo = fopen(fifoname, "r");
	if(fifo)
	{
		fgets(answer, sizeof answer, fifo);
		return 0;
	}

	return 1;
}
int request_serverGet(char* serverFIFO, struct Request *r){
	int received=0;
	char line[MAX_LINE];
	//printf("Server fifo: %s\n", serverFIFO);
	FILE *file = fopen(serverFIFO, "r");
	if(file==NULL) perror("Serverfifo: ");
	if(fgets(line, sizeof line, file)!=NULL){
				received=1;
				printf("Recebeu do cliente: %s\n",line);
		}
	return received;
}
