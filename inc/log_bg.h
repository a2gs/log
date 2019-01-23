#ifndef __A2GS_LOG_BG__
#define __A2GS_LOG_BG__


/* *** INCLUDES *** */
#include <time.h>


/* *** DEFINES *** */
#define a2gs_LOG_REFERENCE_SIZE   50  /* For a2gs_Log_t struct */
#define a2gs_LOG_PATH_SIZE        255 /* For a2gs_Log_t struct */
#define a2gs_LOG_ERRO_STRING_SIZE 128 /* For a2gs_Log_t struct */

#define a2gs_LOG_ERRO_STR1 "Path not defined"
#define a2gs_LOG_ERRO_STR2 "Reference not defined"
#define a2gs_LOG_ERRO_STR3 "Open() erro: %s"
#define a2gs_LOG_ERRO_STR4 "Level not allowed"
#define a2gs_LOG_ERRO_STR5 "Write()erro: %s"
#define a2gs_LOG_ERRO_STR6 "SYSLOG interface yet not developed"
#define a2gs_LOG_ERRO_STR7 "Malloc Error. Cannt allocate buffer to write to log"
#define a2gs_LOG_ERRO_STR8 "DAEMON interface yet not developed"


/* *** DATA TYPES *** */
enum _a2gs_Log_Erro_t{
	a2gs_Log_Ok = 0,
	a2gs_Log_NoPath,
	a2gs_Log_NoReference,
	a2gs_Log_EOpen,
	a2gs_Log_NotDefLevel,
	a2gs_Log_EWrite,
	a2gs_Log_SyslogNotDev,
	a2gs_Log_Malloc,
	a2gs_Log_DaemonNotDev,
	a2gs_Log_Unknow
};

struct _a2gs_Log_t{
	int fd;                                      /* Descriptor */
	unsigned int level;                          /* Level */
	char fileName[a2gs_LOG_PATH_SIZE + 1];       /* Log file name */
	char reference[a2gs_LOG_REFERENCE_SIZE + 1]; /* User name reference */
	struct tm timeOpen, timeClose;               /* When opened and closed */

	enum _a2gs_Log_Erro_t erro;                  /* Erro indicator */
	char erroStr[a2gs_LOG_ERRO_STRING_SIZE + 1]; /* Erro string */

	char *msg;                                   /* Msg to write to log */
	size_t msgSize;                              /* Max size of a message to write */
};


#endif
