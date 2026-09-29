#ifndef INCLUDE_IO_IO_H_
#define INCLUDE_IO_IO_H_

#ifdef __cplusplus
#	define LLPC_IO_OPEN			extern "C" {
#	define LLPC_IO_CLOSE		}
#else
#	define LLPC_IO_OPEN
#	define LLPC_IO_CLOSE
#endif

LLPC_IO_OPEN

typedef enum LLPC_IOError
{
	LLPC_IOE_SystemError = -5,

	LLPC_IOE_OK = 0,

	LLPC_IOE_OutputError,
	LLPC_IOE_InputError,

	LLPC_IOE_NoCommand,

	LLPC_IOE_UNKNOWN,
} LLPC_IOError;

LLPC_IO_CLOSE

#endif  // INCLUDE_IO_IO_H_
