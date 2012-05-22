/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#ifndef LOGS_H_
#define LOGS_H_

extern char * logsFileName;
void logs_changeFileName(char *name);
void logs_addEvent(char *msg, char *who);
void logs_createFile();
int file_exists(char *filename);
void getDate(char *date);
void getHour(char *hour);


#endif /* LOGS_H_ */
