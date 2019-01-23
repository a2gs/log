/*
 * Andre Augusto Giannotti Scota
 * andre.scota@gmail.com
 *
 * Public Domain
 *
 */


/*
 * log.c
 * This file implements the log sistem.
 *
 *
 *  Who     | When       | What
 *  --------+------------+----------------------------
 *   a2gs   | 2005 06 30 | Creation v0.1
 */


/* *** INCLUDES *** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <log.h>
#include <log_bg.h>
#include <sys/types.h>
#include <sys/stat.h>


/* *** DEFINES AND LOCAL DATA TYPE DEFINATION *** */
#define A2GS_LOG_BITWISE_AND_LOG_LEVEL (log->level & logLevel)
#define a2gs_LOG_CLEAR_ERRO(__log_) {                             \
                                      __log_->erro = a2gs_Log_Ok; \
                                      __log_->erroStr[0] = '\0';  \
                                    }
#define a2gs_LOG_SET_ERRO(__log_, __erro_, __erroStr_) {                                      \
                                                         __log_->erro = __erro_;              \
                                                         strcpy(__log_->erroStr, __erroStr_); \
                                                       }


/* *** LOCAL PROTOTYPES *** */
int a2gs_Log_Wellcome(a2gs_Log_t * );
int a2gs_Log_Goodbye(a2gs_Log_t * );


/* *** EXTERNS / LOCAL GLOBALS *** */


/* ------------------------------------------------------------------------------------------- */


/* int a2gs_Log_Goodbye(a2gs_Log_t *log)
 *
 * Write the default log close message.
 *
 * INPUT:
 *  log - Some log environment.
 * OUTPUT:
 *  a2gs_LOG_OK - Message wrotten ok.
 *  a2gs_LOG_ERRO - Erro writting message.
 */
int a2gs_Log_Goodbye(a2gs_Log_t *log)
{
	char msg[512] = {0};
	char msgTime[32] = {0};
	time_t logTimeTimet;

	time(&logTimeTimet);
	memcpy(&(log->timeClose), localtime(&logTimeTimet), sizeof(struct tm));

	strftime(msgTime, 32, "%Yy/%mm/%dd - %Hh:%Mm:%Ss", &(log->timeClose));

	sprintf(msg, "Log closed at [%s]\n" \
	        "-------------------------------------------\n", msgTime);

	if(write(log->fd, msg, strlen(msg)) == -1)
		return(a2gs_LOG_ERRO);

	return(a2gs_LOG_OK);
}


/* int a2gs_Log_Wellcome(a2gs_Log_t *log)
 *
 * Write the default log open message.
 *
 * INPUT:
 *  log - Some log environment.
 * OUTPUT:
 *  a2gs_LOG_OK - Message wrotten ok.
 *  a2gs_LOG_ERRO - Erro writting message.
 */
int a2gs_Log_Wellcome(a2gs_Log_t *log)
{
	char msg[512] = {0};
	char msgTime[32] = {0};

	strftime(msgTime, 32, "%Yy/%mm/%dd - %Hh:%Mm:%Ss", &(log->timeOpen));

	sprintf(msg, "-------------------------------------------\n" \
	        "Log [%s] open at [%s] PID [%d] - User [%s]\n" \
	        "YYYYMMDD HHMMSS MSecond|PID   PPID|Level|Text\n",
	        log->fileName, msgTime, getpid(), getlogin());

	if(write(log->fd, msg, strlen(msg)) == -1)
		return(a2gs_LOG_ERRO);

	return(a2gs_LOG_OK);
}


/* int a2gs_Log_CreateOpenLog(a2gs_Log_t *log, char *path, char *reference)
 *
 * Create/open a log environment.
 *
 * INPUT:
 *  log - Pointer to a a2gs_Log_t memory area.
 *  path - Path where the file will be created/open (append).
 *  reference - A name reference to this log environment.
 * OUTPUT:
 *  a2gs_LOG_OK - Log opening/creating ok.
 *  a2gs_LOG_ERRO - Erro opening/creating log;
 */
int a2gs_Log_CreateOpenLog(a2gs_Log_t *log, char *path, char *reference, size_t maxLogMsgs)
{
	char logOpenTime[25 + 1] = {0};
	time_t logTimeTimet;

	if(path[0] == '\0'){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_NoPath, a2gs_LOG_ERRO_STR1);
		return(a2gs_LOG_ERRO);
	}

	if(reference[0] == '\0'){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_NoReference, a2gs_LOG_ERRO_STR2);
		return(a2gs_LOG_ERRO);
	}

	memset(log, 0, sizeof(a2gs_Log_t));
	a2gs_LOG_CLEAR_ERRO(log);

	log->msg = (char *)malloc(maxLogMsgs);
	if(log->msg == NULL){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_Malloc, a2gs_LOG_ERRO_STR7);
		return(a2gs_LOG_ERRO);
	}

	log->msgSize = maxLogMsgs;
	memset(log->msg, 0, maxLogMsgs);

	time(&logTimeTimet);
 	memcpy(&(log->timeOpen), localtime(&logTimeTimet), sizeof(struct tm));

	/* REFACTORY START: criar uma funcao que me retorne o tipo do log */

	if(strcmp(reference, a2gs_LOG_STD_ERROR_STRING) == 0){
		log->fd = STDERR_FILENO;
		strcpy(log->fileName, "STDERR_FILENO");
		strcpy(log->reference, "Std Erro Out");
		if(a2gs_Log_Wellcome(log) == a2gs_LOG_ERRO){
			char erroStr[a2gs_LOG_ERRO_STRING_SIZE + 1] ={0};

			sprintf(erroStr, a2gs_LOG_ERRO_STR3, strerror(errno));
			a2gs_LOG_SET_ERRO(log, a2gs_Log_EOpen, erroStr);
			return(a2gs_LOG_ERRO);
		}
		return(a2gs_LOG_OK);
	}

	if(strcmp(reference, a2gs_LOG_SYSLOG_STRING) == 0){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_SyslogNotDev, a2gs_LOG_ERRO_STR6);
		return(a2gs_LOG_ERRO);
	}

	if(strcmp(reference, a2gs_LOG_DAEMON_STRING) == 0){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_DaemonNotDev, a2gs_LOG_ERRO_STR8);
		return(a2gs_LOG_ERRO);
	}

	/* REFACTORY END */

	/* REFACTORY START: criar uma funcao que recebe nome do arquivo e ponteiro para descritor */

	strncpy(log->fileName, path, a2gs_LOG_PATH_SIZE);

	strftime(logOpenTime, 25, "%Y%m%d", &(log->timeOpen));

	if(log->fileName[strlen(log->fileName)-1] != '/')
		sprintf(&(log->fileName[strlen(log->fileName)]), "/%s_%s.log", reference, logOpenTime);
	else
		sprintf(&(log->fileName[strlen(log->fileName)]), "%s_%s.log", reference, logOpenTime);

	if((log->fd = open((const char *)log->fileName, O_WRONLY|O_CREAT|O_APPEND|O_NONBLOCK,
	                   S_IRUSR|S_IWUSR|S_IRGRP)) == -1){
		char erroStr[a2gs_LOG_ERRO_STRING_SIZE + 1] ={0};

		sprintf(erroStr, a2gs_LOG_ERRO_STR3, strerror(errno));
		a2gs_LOG_SET_ERRO(log, a2gs_Log_EOpen, erroStr);
		return(a2gs_LOG_ERRO);
	}

	/* REFACTORY END */

	if(a2gs_Log_Wellcome(log) == a2gs_LOG_ERRO){
		char erroStr[a2gs_LOG_ERRO_STRING_SIZE + 1] ={0};

		sprintf(erroStr, a2gs_LOG_ERRO_STR3, strerror(errno));
		a2gs_LOG_SET_ERRO(log, a2gs_Log_EOpen, erroStr);
		return(a2gs_LOG_ERRO);
	}

	return(a2gs_LOG_OK);
}


/* int a2gs_Log_WriteLog(a2gs_Log_t *log, int logLevel, char *msg, ...)
 *
 * Write a message to a log environment.
 *
 * INPUT:
 *  log - Some log environment.
 *  logLevel - Message level.
 *  msg - Message format.
 *  ... - Parameters.
 * OUTPUT:
 *  a2gs_LOG_OK - Message wrotten ok.
 *  a2gs_LOG_ERRO - Erro writting message.
 */
int a2gs_Log_WriteLog(a2gs_Log_t *log, int logLevel, char *msg, ...)
{
	int retPrint = 0;
	struct tm logTimeTm;
	time_t logTimeTimet;
	va_list args;
	struct timeval tv;
	struct timezone tz;

	a2gs_LOG_CLEAR_ERRO(log);

	if(!A2GS_LOG_BITWISE_AND_LOG_LEVEL){
		a2gs_LOG_SET_ERRO(log, a2gs_Log_EWrite, a2gs_LOG_ERRO_STR4);
		return(a2gs_LOG_ERRO);
	}

	time(&logTimeTimet);
	memcpy(&logTimeTm, localtime(&logTimeTimet), sizeof(struct tm));
	strftime(log->msg, log->msgSize, "%Y%m%d %H%M%S", &logTimeTm);

	if(gettimeofday(&tv, &tz) == 0){
		sprintf(&(log->msg[15]), " %07ld|", tv.tv_usec);
	}

	sprintf(&(log->msg[24]), "%d %d|", getpid(), getppid());

	if(a2gs_LOG_LEVEL_RED_ALERT & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "RED ");

	if(a2gs_LOG_LEVEL_MUST_LOG_IT & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "LOG ");

	if(a2gs_LOG_LEVEL_DATABASE_ALERT & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "DBE ");

	if(a2gs_LOG_LEVEL_OPERATOR_ALERT & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "OPA ");

	if(a2gs_LOG_LEVEL_DATABASE_MESSAGE & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "DBM ");

	if(a2gs_LOG_LEVEL_OPERATOR_MESSAGE & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "OPM ");

	if(a2gs_LOG_LEVEL_MESSAGE & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "MSG ");

	if(a2gs_LOG_LEVEL_DEVELOP & A2GS_LOG_BITWISE_AND_LOG_LEVEL)
		strcat(log->msg, "DEV ");

	retPrint = strlen(log->msg);

	log->msg[retPrint - 1] = '|';

	va_start(args, msg);

	retPrint += vsnprintf(&(log->msg[retPrint]), log->msgSize - retPrint, msg, args);

	va_end(args);

	log->msg[retPrint++] = '\n';

	if(write(log->fd, log->msg, retPrint) == -1){
		char erroStr[a2gs_LOG_ERRO_STRING_SIZE + 1] ={0};

		sprintf(erroStr, a2gs_LOG_ERRO_STR5, strerror(errno));
		a2gs_LOG_SET_ERRO(log, a2gs_Log_EWrite, erroStr);
		return(a2gs_LOG_ERRO);
	}

	return(a2gs_LOG_OK);
}


/* int a2gs_Log_CloseLog(a2gs_Log_t *log)
 *
 * Close a log environment.
 *
 * INPUT:
 *  log - Some log environment.
 * OUTPUT:
 *  a2gs_LOG_OK - Log closed.
 *  a2gs_LOG_ERRO - Problem closeing log.
 */
int a2gs_Log_CloseLog(a2gs_Log_t *log)
{
	free(log->msg);

	a2gs_LOG_CLEAR_ERRO(log);
	a2gs_Log_Goodbye(log);

	close(log->fd);
	return(a2gs_LOG_OK);
}
