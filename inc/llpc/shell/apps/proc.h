#ifndef INCLUDE_APPS_PROC_H_
#define INCLUDE_APPS_PROC_H_

#ifdef __cplusplus
#	define LLPC_SH_PROC_OPEN			extern "C" {
#	define LLPC_SH_PROC_CLOSE			}
#else
#	define LLPC_SH_PROC_OPEN
#	define LLPC_SH_PROC_CLOSE
#endif

LLPC_SH_PROC_OPEN
LLPC_SH_PROC_CLOSE

#endif  // INCLUDE_APPS_PROC_H_
