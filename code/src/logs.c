/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include "logs.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_FILENAME_LEN 100
#define MAX_HEADER_LEN 100
#define MAX_BUFFER_SIZE 100
#define MAX_MESSAGE_LEN 42
char * logsFileName="/tmp/logfile.txt";


void getDate(char *date){
	time_t now;
	struct tm *d;
	char li[13];
	time(&now);
	d = localtime(&now);
	strftime(li, 15, "%d/%m/%Y", d);
	strcpy(date, li);
}

void getHour(char *hour){
	time_t epoch_time;
	struct tm *tm_p;
	epoch_time = time( NULL );
	tm_p = localtime( &epoch_time );
	sprintf(hour,"%.2d:%.2d:%.2d",tm_p->tm_hour, tm_p->tm_min, tm_p->tm_sec );
}
void logs_changeFileName(char *name){
	strcpy(logsFileName,name);
}
void logs_addEvent(char *msg, char *who){
	if(!file_exists(logsFileName)) logs_createFile();

	// Get the date and hour and allocate memory
	char *buffer=malloc(sizeof(char)*MAX_BUFFER_SIZE);
	char *hour=malloc(sizeof(char)*15);
	char *date=malloc(sizeof(char)*15);
	getDate(date);
	getHour(hour);

	// Shorten message if necessary
	if(strlen(msg)>MAX_MESSAGE_LEN){
		char *shortenMsg=malloc(sizeof(char)*(MAX_MESSAGE_LEN+1));
		strncpy(shortenMsg,msg,MAX_MESSAGE_LEN);
		msg=shortenMsg;
		strcat(msg,"...");
	}

	// Print to buffer
	sprintf(buffer,"%-10s %-8s %-8s %s\n",date,hour,who,msg);

	// Open file and append
	int fileDes=open(logsFileName,O_WRONLY | O_APPEND);
	if(fileDes==-1) perror("adicionar evento");
	write(fileDes,buffer,strlen(buffer)*sizeof(char));
	close(fileDes);


}
void logs_createFile(){
	int fileDes=open(logsFileName,O_CREAT | O_WRONLY,0777);
	if(fileDes==-1) perror("logfile creation\n");
	char *header=malloc(sizeof(char)*MAX_HEADER_LEN);
	sprintf(header,"   DATA      HORA   PROGRAMA OPERACAO\n");
	write(fileDes,header,strlen(header)*sizeof(char));
	close(fileDes);
}
int file_exists(char *filename){
	return (open(filename, O_RDONLY)!=-1);
}
