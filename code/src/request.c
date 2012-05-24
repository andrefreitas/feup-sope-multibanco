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

int request_writeFIFO(char* fifoname,struct Request* r,char *msg){
	FILE *fifo = fopen(fifoname, "a+");
	if (fifo) {
		char wrStr[128];

		// If is a request
		if(r!=NULL)
			sprintf(wrStr, "%s: %d > %s\n", r->who, r->pid, r->request);

		// If is a common message
		if(msg!=NULL)
			strcpy(wrStr,msg);

		// write in fifo
		fwrite(wrStr, sizeof(char) * strlen(wrStr), 1, fifo);

		return 0;
	}
	fclose(fifo);
	return 1;
}

int request_waitFIFO(char* fifoname,struct Request* r,char *msg ) {

	return 0;
}

int request_readFIFO(char* fifoname,struct Request* r,char *msg){
	int received=0;
	char line[MAX_LINE];
	//printf("Server fifo: %s\n", serverFIFO);
	FILE *file = fopen(fifoname, "r");
	if(file==NULL) perror("Serverfifo: ");
	if(fgets(line, sizeof line, file)!=NULL){
				received=1;
				strcpy(msg,line);
				printf("Recebeu do cliente: %s\n",line);
		}
	fclose(file);
	return received;
}
