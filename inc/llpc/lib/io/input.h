#ifndef INCLUDE_IO_INPUT_H_
#define INCLUDE_IO_INPUT_H_

#include <efi/efi.h>

#ifdef __cplusplus
#	define LLPC_INPUT_OPEN			extern "C" {
#	define LLPC_INPUT_CLOSE			}
#else
#	define LLPC_INPUT_OPEN
#	define LLPC_INPUT_CLOSE
#endif

LLPC_INPUT_OPEN

EFI_STATUS llpc_io_extReadline(CHAR16 *buff, const UINTN cap, const CHAR16 del);

LLPC_INPUT_CLOSE

#endif  // INCLUDE_IO_INPUT_H_

