#ifndef INCLUDE_APPS_EXIT_H_
#define INCLUDE_APPS_EXIT_H_

#ifdef __cplusplus
#	define LLPC_SH_EXIT_OPEN			extern "C" {
#	define LLPC_SH_EXIT_CLOSE			}
#else
#	define LLPC_SH_EXIT_OPEN
#	define LLPC_SH_EXIT_CLOSE
#endif

LLPC_SH_EXIT_OPEN
LLPC_SH_EXIT_CLOSE

#endif  // INCLUDE_APPS_EXIT_H_

