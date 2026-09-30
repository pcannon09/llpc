#ifndef INCLUDE_IO_OUTPUT_H_
#define INCLUDE_IO_OUTPUT_H_

#include <efi/efi.h>

#include "llpc/lib/io/io.h"
#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_OUTPUT_OPEN			extern "C" {
#	define LLPC_OUTPUT_CLOSE		}
#else
#	define LLPC_OUTPUT_OPEN
#	define LLPC_OUTPUT_CLOSE
#endif

LLPC_OUTPUT_OPEN

typedef enum LLPC_OutputType
{
	LLPC_OT_Clear,
	LLPC_OT_SetCursorState,
	LLPC_OT_CurGoto, // Cursor Goto
} LLPC_OutputType;

LLPCAPI LLPC_IOError llpc_io_echo(CHAR16 *message);
LLPCAPI LLPC_IOError llpc_io_print(CHAR16 *message);

LLPCAPI LLPC_IOError llpc_io_action(const LLPC_OutputType action, void *p1, ...);

LLPC_OUTPUT_CLOSE

#endif  // INCLUDE_IO_OUTPUT_H_

