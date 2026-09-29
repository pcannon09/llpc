#ifndef INCLUDE_TIME_TIME_H_
#define INCLUDE_TIME_TIME_H_

#include <efi/efi.h>

#include "llpc/lib/time/datetime.h"
#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_TIME_OPEN			extern "C" {
#	define LLPC_TIME_CLOSE			}
#else
#	define LLPC_TIME_OPEN
#	define LLPC_TIME_CLOSE
#endif

LLPC_TIME_OPEN

#ifdef __LLPC_MILLISECOND_SIZE
# 	undef __LLPC_MILLISECOND_SIZE
#endif

#define __LLPC_MILLISECOND_SIZE 		1000000

typedef struct LLPC_Time
{
	UINT32 nanosecond;
	UINT32 millisecond;

	UINT8 hour;
	UINT8 minute;
	UINT8 second;
} LLPC_Time;

LLPCAPI LLPC_DTStatus llpc_time_get(LLPC_Time *time);

LLPC_TIME_CLOSE

#endif  // INCLUDE_TIME_TIME_H_

