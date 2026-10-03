#ifndef INCLUDE_IO_INPUT_H_
#define INCLUDE_IO_INPUT_H_

#include <efi/efi.h>

#include "llpc/lib/types.h"
#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_INPUT_OPEN			extern "C" {
#	define LLPC_INPUT_CLOSE			}
#else
#	define LLPC_INPUT_OPEN
#	define LLPC_INPUT_CLOSE
#endif

LLPC_INPUT_OPEN

LLPCAPI EFI_INPUT_KEY llpc_io_extReadchar(void);
EFI_STATUS llpc_io_extReadline(CHAR16 **buff, const int cap, const CHAR16 del, const llpc_bool dynamic);

#define llpc_io_readchar 		llpc_io_extReadchar().UnicodeChar

LLPC_INPUT_CLOSE

#endif  // INCLUDE_IO_INPUT_H_

