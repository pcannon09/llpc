#ifndef INCLUDE_TIME_DATE_H_
#define INCLUDE_TIME_DATE_H_

#include <efi/efi.h>

#include "llpc/lib/time/datetime.h"
#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_DATE_OPEN			extern "C" {
#	define LLPC_DATE_CLOSE			}
#else
#	define LLPC_DATE_OPEN
#	define LLPC_DATE_CLOSE
#endif

LLPC_DATE_OPEN

typedef struct LLPC_Date 
{
	UINT16 year; // 1998 - 20XX

	UINT8 day;
	UINT8 month;
} LLPC_Date;

LLPCAPI LLPC_DTStatus llpc_date_get(LLPC_Date *date);

LLPC_DATE_CLOSE

#endif  // INCLUDE_TIME_DATE_H_

