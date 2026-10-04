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

#define LLPC_PROC_DENY_IF_IMPL(_procObj, _symbol) \
	(_procObj _symbol obj.currentSig == LLPC_PSIG_PAUSE || \
	 _procObj _symbol obj.currentSig == LLPC_PSIG_BG)

#define LLPC_PROC_DENY_IF(_procObj) 	LLPC_PROC_DENY_IF_IMPL(_procObj, ->)

typedef uint32_t 		LLPC_PID;

typedef enum LLPC_ProcSignals
{
	__LLPC_PSIG_START = -5,

	LLPC_PSIG_NONE = 0, // Or also: Reset State

	LLPC_PSIG_KILL,
	LLPC_PSIG_CLEAN,

	LLPC_PSIG_BG,
	LLPC_PSIG_FG,
	LLPC_PSIG_PAUSE = LLPC_PSIG_BG,
} LLPC_ProcSignals;

typedef enum LLPC_ProcessError
{
	LLPC_PROC_ERR_OK = 0,

	LLPC_PROC_ERR_InitError,
	LLPC_PROC_ERR_VectorAction,
	LLPC_PROC_PIDNotFound,
	LLPC_PROC_InvalidData,

	LLPC_PROC_ERR_NULL,
} LLPC_ProcessError;

typedef struct LLPC_ProcessSector
{
	void *data;
	char *id;

	LLPC_ProcessError initError;

	LLPC_PID pid;
	LLPC_ProcSignals sig;
} LLPC_ProcessSector;

typedef struct LLPC_SignalSupportStatus
{
	const char *id;

	LLPC_ProcSignals currentSig;
	LLPC_ProcessSector sector;
} LLPC_SignalSupportStatus;

typedef struct LLPC_GlobalProcessInfo
{
	LLPC_Vector sectorList; // TYPE: LLPC_ProcessSector
	LLPC_ProcessError lastError;

	LLPC_PID lastPID;
	LLPC_PID startPID;

	llpc_bool __initialized;
} LLPC_GlobalProcessInfo;

extern LLPC_GlobalProcessInfo llpc_processData;

LLPC_GlobalProcessInfo llpc_proc_init(void);

LLPC_ProcessSector llpc_proc_initSector(LLPC_GlobalProcessInfo *gpi, const char *name,
		const LLPC_ProcessSector *sector);
LLPC_ProcessSector llpc_proc_getSectorByPID(LLPC_GlobalProcessInfo *gpi, const LLPC_PID pid);
LLPC_ProcessSector llpc_proc_getSectorByName(LLPC_GlobalProcessInfo *gpi, const char *name);

LLPC_ProcessError llpc_proc_destroy(LLPC_GlobalProcessInfo *gpi);
LLPC_ProcessError llpc_proc_destroySector(LLPC_GlobalProcessInfo *gpi, const LLPC_PID pid);

LLPC_CPP_PROCESS_CLOSE

#endif  // INCLUDE_SIGNALS_SIGNALS_H_

