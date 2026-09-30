#ifndef INCLUDE_TIME_SLEEP_H_
#define INCLUDE_TIME_SLEEP_H_

#include <efi/efi.h>

#ifdef __cplusplus
#	define LLPC_SLEEP_OPEN			extern "C" {
#	define LLPC_SLEEP_CLOSE			}
#else
#	define LLPC_SLEEP_OPEN
#	define LLPC_SLEEP_CLOSE
#endif

LLPC_SLEEP_OPEN

EFI_STATUS llpc_sleepMS(const UINT64 milliseconds);

LLPC_SLEEP_CLOSE

#endif  // INCLUDE_TIME_SLEEP_H_

