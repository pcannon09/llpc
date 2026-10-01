#ifndef INCLUDE_SIGNALS_SIGNALS_H_
#define INCLUDE_SIGNALS_SIGNALS_H_

#include <stdint.h>

#include "llpc/lib/vector/vector.h"

#ifdef __cplusplus
# 	define LLPC_CPP_PROCESS_OPEN 		extern "C" {
# 	define LLPC_CPP_PROCESS_CLOSE 		}
#else
# 	define LLPC_CPP_PROCESS_OPEN
# 	define LLPC_CPP_PROCESS_CLOSE
#endif

LLPC_CPP_PROCESS_OPEN

typedef enum LLPC_ProcSignals
{
	LLPC_PSIG_NONE = 0,
	LLPC_PSIG_ERROR,

	LLPC_PSIG_STOP,
	LLPC_PSIG_KILL,

	LLPC_PSIG_BG,
	LLPC_PSIG_FG,
} LLPC_ProcSignals;

typedef uint32_t 		LLPC_PID;

typedef enum LLPC_ProcessError
{
	LLPC_PROC_ERR_OK = 0,

	LLPC_PROC_ERR_InitError,
	LLPC_PROC_ERR_VectorAction,
	LLPC_PROC_PIDNotFound,

	LLPC_PROC_ERR_NULL,
} LLPC_ProcessError;

typedef struct LLPC_ProcessSector
{
	char *name;

	LLPC_ProcessError initError;

	LLPC_PID pid;
	LLPC_ProcSignals sig;
} LLPC_ProcessSector;

typedef struct LLPC_GlobalProcessInfo
{
	LLPC_Vector sectorList; // TYPE: LLPC_ProcessSector
	LLPC_ProcessError lastError;

	llpc_bool __initialized;
} LLPC_GlobalProcessInfo;

extern LLPC_GlobalProcessInfo llpc_processData;

LLPC_GlobalProcessInfo llpc_proc_init(void);

LLPC_ProcessSector llpc_proc_initSector(LLPC_GlobalProcessInfo *gpi, void *data);

LLPC_ProcessError llpc_proc_destroy(LLPC_GlobalProcessInfo *gpi);
LLPC_ProcessError llpc_proc_destroySector(LLPC_GlobalProcessInfo *gpi, const LLPC_PID pid);

LLPC_CPP_PROCESS_CLOSE

#endif  // INCLUDE_SIGNALS_SIGNALS_H_

