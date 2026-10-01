#ifndef INCLUDE_SCREEN_SCREEN_H_
#define INCLUDE_SCREEN_SCREEN_H_

#include <efi/efi.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
#	define LLPC_SCREEN_OPEN		extern "C" {
#	define LLPC_SCREEN_CLOSE		}
#else
#	define LLPC_SCREEN_OPEN
#	define LLPC_SCREEN_CLOSE
#endif

LLPC_SCREEN_OPEN

typedef struct LLPC_Screen
{
	unsigned int curX, curY;

	UINT8 screenColor;
	UINT8 screenTextColor;

	llpc_bool __initialized;
} LLPC_Screen;

typedef enum LLPC_ScreenStatus
{
	LLPC_SSTAT_OK = 0,

	LLPC_SSTAT_IOE_Error,
	LLPC_SSTAT_NotInitialized,

	LLPC_SSTAT_UpdateFailed,
} LLPC_ScreenStatus;

LLPC_Screen llpc_screen_init(void);

LLPC_ScreenStatus llpc_screen_update(LLPC_Screen *screen, llpc_bool clear);

llpc_bool llpc_screen_destroy(LLPC_Screen *screen);

LLPC_SCREEN_CLOSE

#endif  // INCLUDE_SCREEN_SCREEN_H_

