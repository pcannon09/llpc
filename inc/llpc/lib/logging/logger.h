#ifndef INCLUDE_LOGGING_LOGGER_H_
#define INCLUDE_LOGGING_LOGGER_H_

#include <efi/efi.h>
#include <efi/efilib.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
#	define LLPC_LOGGER_OPEN				extern "C" {
#	define LLPC_LOGGER_CLOSE			}
#else
#	define LLPC_LOGGER_OPEN
#	define LLPC_LOGGER_CLOSE
#endif

LLPC_LOGGER_OPEN

#define LLPC_LOG_LVLCHECK(_level) 		(!(_level < llpc_appData.logLevel))

typedef enum LLPC_LogLevel
{
	LLPC_LL_None = 0,

	LLPC_LL_Extra	= 5,

	LLPC_LL_Debug 	= 10,
	LLPC_LL_Verbose = 20,
	LLPC_LL_Log 	= 30,
	LLPC_LL_Warning = 40,
	LLPC_LL_Error 	= 50,
	LLPC_LL_Fatal 	= 60,
} LLPC_LogLevel;

LLPCAPI void llpc_logecho(const LLPC_LogLevel level, CHAR16 *msg);
LLPCAPI void llpc_extlog(const LLPC_LogLevel level, const char *msg, const llpc_bool newline);

#define llpc_log(_lvl, _msg) \
	llpc_extlog(_lvl, _msg, llpctrue)

LLPC_LOGGER_CLOSE

#endif  // INCLUDE_LOGGING_LOGGER_H_

