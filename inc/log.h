#ifndef __A2GS_LOG__
#define __A2GS_LOG__


/* *** INCLUDES *** */
#include <log_bg.h>


/* *** DEFINES *** */
#define a2gs_LOG_OK   1
#define a2gs_LOG_ERRO 0

#define a2gs_LOG_STD_ERROR_STRING "STD ERROR" /* The reference if user wants send log to Standart Erro */
#define a2gs_LOG_SYSLOG_STRING    "SYSLOG"    /* The reference if user wants send log to SYSLOG */
#define a2gs_LOG_DAEMON_STRING    "DAEMON"    /* The reference if user wants send log to daemon */

#define a2gs_LOG_LEVEL_MUST_LOG_IT        (unsigned int)0x1  /* 00000001 */
#define a2gs_LOG_LEVEL_RED_ALERT          (unsigned int)0x2  /* 00000010 */
#define a2gs_LOG_LEVEL_DATABASE_ALERT     (unsigned int)0x4  /* 00000100 */
#define a2gs_LOG_LEVEL_DATABASE_MESSAGE   (unsigned int)0x8  /* 00001000 */
#define a2gs_LOG_LEVEL_OPERATOR_ALERT     (unsigned int)0x10 /* 00010000 */
#define a2gs_LOG_LEVEL_OPERATOR_MESSAGE   (unsigned int)0x20 /* 00100000 */
#define a2gs_LOG_LEVEL_MESSAGE            (unsigned int)0x40 /* 01000000 */
#define a2gs_LOG_LEVEL_DEVELOP            (unsigned int)0x80 /* 10000000 */

#define a2gs_LOG_GET_ERRO(__log_)            (__log_.erro) /* To get last erro happened */
#define a2gs_LOG_GET_ERROSTRING(__log_)      (__log_.erroStr) /* To get last erro message happened */
#define a2gs_LOG_SET_LEVEL(__log_, __level_) (__log_.level = __level_ | a2gs_LOG_LEVEL_MUST_LOG_IT) /* To set level */


/* *** DATA TYPES *** */
typedef enum _a2gs_Log_Erro_t a2gs_Log_Erro_t;
typedef struct _a2gs_Log_t a2gs_Log_t;


/* *** INTERFACES / PROTOTYPES *** */
/* int a2gs_Log_CreateOpenLog(a2gs_Log_t *log, char *path, char *reference, size_t maxLogMsgs);
 *
 * Create/open a log environment.
 *
 * INPUT:
 *  log - Pointer to a a2gs_Log_t memory area.
 *  path - Path where the file will be created/open (append).
 *  reference - A name reference to this log environment.
 *              It may be any user's name or a2gs_LOG_STD_ERROR_STRING or a2gs_LOG_SYSLOG_STRING or
 *              a2gs_LOG_DAEMON_STRING.
 *  maxLogMsgs - The max size of messages that will be send to log.
 * OUTPUT:
 *  a2gs_LOG_OK - Log opening/creating ok.
 *  a2gs_LOG_ERRO - Erro opening/creating log;
 */
int a2gs_Log_CreateOpenLog(a2gs_Log_t * , char * , char * , size_t);

/* int a2gs_Log_CloseLog(a2gs_Log_t * )
 *
 * Close a log environment.
 *
 * INPUT:
 *  log - Some log environment.
 * OUTPUT:
 *  a2gs_LOG_OK - Log closed.
 *  a2gs_LOG_ERRO - Problem closeing log.
 */
int a2gs_Log_CloseLog(a2gs_Log_t * );

/* int a2gs_Log_WriteLog(a2gs_Log_t * , int , char * , ...)
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
int a2gs_Log_WriteLog(a2gs_Log_t * , int , char * , ...);


/* *** EXAMPLE *** */
	#if 0
		#include <log.h>
		...
		a2gs_Log_t logSystem;
		...
		a2gs_Log_CreateOpenLog(&logSystem, "/tmp", "theLogSystem", 2048);
		...
		a2gs_LOG_SET_LEVEL(logSystem, a2gs_LOG_LEVEL_RED_ALERT|a2gs_LOG_LEVEL_OPERATOR_ALERT);
		...
		a2gs_Log_WriteLog(&logSystem, a2gs_LOG_LEVEL_OPERATOR_ALERT|a2gs_LOG_LEVEL_MESSAGE, "message %d", 13);
		...
		a2gs_Log_CloseLog(&logSystem);
		...
	#endif

#endif
