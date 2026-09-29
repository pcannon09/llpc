#ifndef INCLUDE_LIB_GLOBALS_H_
#define INCLUDE_LIB_GLOBALS_H_

#include <efi/efi.h>
#include <efi/efilib.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
#	define LLPC_GLOBALS_OPEN			extern "C" {
#	define LLPC_GLOBALS_CLOSE		}
#else
#	define LLPC_GLOBALS_OPEN
#	define LLPC_GLOBALS_CLOSE
#endif

LLPC_GLOBALS_OPEN

// ("EP"): Error Protection
#define LLPC_EP_FAIL_EXPR 	!LLPC_ImageHandle || !LLPC_SystemTable
#define LLPC_UEFI_VERSION(major, minor) (((UINT32)(major) << 16) | ((UINT32)(minor) & 0xFFFF))

typedef enum LLPC_LogLevel
{
	LLPC_LL_None = 0,

	LLPC_LL_Verbose,
	LLPC_LL_Log,
	LLPC_LL_Warning,
	LLPC_LL_Error,
	LLPC_LL_Fatal,
} LLPC_LogLevel;

typedef struct LLPC_AppData
{
	const char *name;
	const char *shortName;

	UINT32 version;

	LLPC_LogLevel logLevel;
} LLPC_AppData;

extern LLPC_AppData llpc_appData;
extern EFI_HANDLE LLPC_ImageHandle;
extern EFI_SYSTEM_TABLE *LLPC_SystemTable;

LLPCAPI LLPC_SystemError llpc_initialize(const LLPC_AppData appdata,
		EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

LLPC_GLOBALS_CLOSE

#endif  // INCLUDE_LIB_GLOBALS_H_
