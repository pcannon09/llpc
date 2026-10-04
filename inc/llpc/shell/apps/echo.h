#ifndef INCLUDE_APPS_ECHO_H_
#define INCLUDE_APPS_ECHO_H_

#include "llpc/shell/shell.h"

#ifdef __cplusplus
#	define LLPC_SH_ECHO_OPEN			extern "C" {
#	define LLPC_SH_ECHO_CLOSE			}
#else
#	define LLPC_SH_ECHO_OPEN
#	define LLPC_SH_ECHO_CLOSE
#endif

LLPC_SH_ECHO_OPEN

LLPC_APP_DECL(llpc_app_echo);
LLPC_APP_DECL(llpc_app_echo_impl, const LLPC_PID pid);

LLPC_SH_ECHO_CLOSE

#endif  // INCLUDE_APPS_ECHO_H_
