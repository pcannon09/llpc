#ifndef INCLUDE_APPS_SLEEP_H_
#define INCLUDE_APPS_SLEEP_H_

#include "llpc/shell/shell.h"

#ifdef __cplusplus
#	define LLPC_SH_SLEEP_OPEN			extern "C" {
#	define LLPC_SH_SLEEP_CLOSE			}
#else
#	define LLPC_SH_SLEEP_OPEN
#	define LLPC_SH_SLEEP_CLOSE
#endif

LLPC_SH_SLEEP_OPEN

LLPC_APP_DECL(llpc_app_sleep);
LLPC_APP_DECL(llpc_app_sleep_impl, const LLPC_PID pid);

LLPC_SH_SLEEP_CLOSE

#endif  // INCLUDE_APPS_SLEEP_H_
