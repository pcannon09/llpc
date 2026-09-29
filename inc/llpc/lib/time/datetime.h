#ifndef INCLUDE_TIME_DATETIME_H_
#define INCLUDE_TIME_DATETIME_H_

#ifdef __cplusplus
#	define LLPC_DATETIME_OPEN			extern "C" {
#	define LLPC_DATETIME_CLOSE			}
#else
#	define LLPC_DATETIME_OPEN
#	define LLPC_DATETIME_CLOSE
#endif

LLPC_DATETIME_OPEN

typedef enum LLPC_DTStatus
{
	LLPC_DTS_OK,
	LLPC_DTS_SystemError,

	LLPC_DTS_FailTime,
	LLPC_DTS_FailDate,

	LLPC_DTS_EFI_StatusFailed,
} LLPC_DTStatus;

LLPC_DATETIME_CLOSE

#endif  // INCLUDE_TIME_DATETIME_H_

