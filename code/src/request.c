#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	if (line == "OK" || line == "OK\n")
		return 1;

	if (line == "FAIL" || line == "FAIL\n")
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
