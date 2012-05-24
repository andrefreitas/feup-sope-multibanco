#ifndef REQUEST_H_
#define REQUEST_H_
struct Request {
	unsigned int pid;
	char* who;
	char* request;
};

void request_create(struct Request* r, unsigned int pid, char* who, char* request);

int request_writeFIFO(char* fifoname,struct Request* r,char *msg);

int request_readFIFO(char* fifoname,struct Request* r,char *msg);

int request_waitFIFO(char* fifoname,struct Request* r,char *msg );

#endif
