#ifndef REQUEST_H_
#define REQUEST_H_
struct Request {
	unsigned int pid;
	char* who;
	char* request;
};

void request_create(struct Request* r, unsigned int pid, char* who, char* request);

int request_send(struct Request* r, char* fifoname);

int request_waitAnswer(char* fifoname);

int request_writeAnswer(char* fifoname, char* answer);

int request_readAnswer(char* fifoname, char* answer);

int request_serverGet(char* serverFIFO, struct Request *r);

#endif
