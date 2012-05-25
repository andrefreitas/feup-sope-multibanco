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

int request_writeFIFO(char* fifoname, struct Request* r, char *msg) {
	FILE *fifo = fopen(fifoname, "a+");
	if (fifo) {
		char wrStr[MAX_LINE];

		// If is a request
		if (r != NULL) {
			sprintf(wrStr, "%s: %d > %s\n", r->who, r->pid, r->request);
			//printf("SENT REQUEST: %s\n", wrStr);
		}

		// If is a common message
		if (msg != NULL) {
			strcpy(wrStr, msg);
			//printf("ANSWERED: %s\n", wrStr);
		}

		// write in fifo
		fwrite(wrStr, sizeof(char) * strlen(wrStr), 1, fifo);

		fclose(fifo);

		return 0;
	}
	return 1;
}

int request_waitFIFO(char* fifoname, struct Request* r, char *msg) {
	FILE *file = fopen(fifoname, "r");
	char line[1000];
	if (file == NULL)
		perror("Ansfifo: ");
	do {
		fgets(line, sizeof(line), file);
	} while (line == NULL);
	strcpy(msg, line);
	//printf("GOT ANSWER: %s\n", msg);
	fclose(file);
	return 0;
}

int request_readFIFO(char* fifoname, struct Request* r, char *msg) {
	int received = 0;
	char line[MAX_LINE];
//printf("Server fifo: %s\n", serverFIFO);
	FILE *file = fopen(fifoname, "r");
	if (file == NULL)
		perror("Serverfifo: ");
	if (fgets(line, sizeof(line), file) != NULL) {
		received = 1;
		if (strchr(line, '>') == NULL) {
			//printf("MESSAGE: %s\n", line);
			strcpy(msg, line);
		} else {
			//printf("GOT REQUEST: %s\n", line);
			char* tmp = malloc(sizeof(char) * MAX_LINE);
			strcpy(tmp, line);
			char* pid;
			char* who;
			char* req;
			who = strtok(tmp, ": >\n");
			pid = strtok(NULL, ": >\n");
			strtok(NULL, " "); //discard space
			req = strtok(NULL, ">\n");
			request_create(r, atoi(pid), who, req);
		}
	}
	fclose(file);
	return received;
}
