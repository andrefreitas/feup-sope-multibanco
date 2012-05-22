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
	if(!file_exists(logsFileName)){
		logs_createFile();
		logs_addHeader();
	}
	char
	char *buffer=malloc

}
void logs_createFile(){
	int fileDes=open(logsFileName,O_CREAT,0777);
	if(fileDes==-1) perror("logfile\n");
	close(fileDes);
}

void logs_addHeader(){
	int fileDes=open(logsFileName,O_WRONLY | O_APPEND | O_TRUNC);
	if(fileDes==-1) perror("logfile\n");
	char *header=malloc(sizeof(char)*MAX_HEADER_LEN);
	sprintf(header,"   DATA      HORA   PROGRAMA  OPERACAO");
	write(fileDes,header,40);
	close(fileDes);

}
int file_exists(char *filename){
	return (open(filename, O_RDONLY)!=-1);
}
