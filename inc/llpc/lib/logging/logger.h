#ifndef INCLUDE_LOGGING_LOGGER_H_
#define INCLUDE_LOGGING_LOGGER_H_

#include "llpc/lib/globals.h"
#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_LOGGER_OPEN				extern "C" {
#	define LLPC_LOGGER_CLOSE			}
#else
#	define LLPC_LOGGER_OPEN
#	define LLPC_LOGGER_CLOSE
#endif

LLPC_LOGGER_OPEN

LLPCAPI void llpc_logecho(CHAR16 *msg);
LLPCAPI void llpc_extlog(const LLPC_LogLevel level, const char *msg, const llpc_bool newline);

#define llpc_log(_lvl, _msg) \
	llpc_extlog(_lvl, _msg, llpctrue)

LLPC_LOGGER_CLOSE

#endif  // INCLUDE_LOGGING_LOGGER_H_

