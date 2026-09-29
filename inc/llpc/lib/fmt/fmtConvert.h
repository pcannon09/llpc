#ifndef INCLUDE_FMT_FMTCONVERT_H_
#define INCLUDE_FMT_FMTCONVERT_H_

#include <efi/x86_64/efibind.h>
#include <efi/efi.h>

#include "llpc/predefines.h"

#ifdef __cplusplus
#	define LLPC_FMTCONVERT_OPEN			extern "C" {
#	define LLPC_FMTCONVERT_CLOSE			}
#else
#	define LLPC_FMTCONVERT_OPEN
#	define LLPC_FMTCONVERT_CLOSE
#endif

LLPC_FMTCONVERT_OPEN

LLPCAPI CHAR16 *llpc_toChar16(CHAR16 *dst, const char *src);

LLPC_FMTCONVERT_CLOSE

#endif  // INCLUDE_FMT_FMTCONVERT_H_
