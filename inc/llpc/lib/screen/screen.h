#ifndef INCLUDE_SCREEN_SCREEN_H_
#define INCLUDE_SCREEN_SCREEN_H_

#include <efi/efi.h>

#include "llpc/lib/process/process.h"
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
	LLPC_SignalSupportStatus obj;

	LLPC_ProcessSector psec;

	unsigned int curX, curY;

	UINT8 screenColor;
	UINT8 screenTextColor;

	llpc_bool __initialized;
} LLPC_Screen;

typedef enum LLPC_ScreenStatus
{
	__LLPC_SSTAT_START = -5,
	LLPC_SSTAT_OK = 0,
	LLPC_SSTAT_Invalid = 0,

	LLPC_SSTAT_IOE_Error,
	LLPC_SSTAT_NotInitialized,
	LLPC_SSTAT_DestroyFail,

	LLPC_SSTAT_UpdateFailed,
	LLPC_SSTAT_EventFailed,
	LLPC_SSTAT_SigDenied,
} LLPC_ScreenStatus;

LLPC_Screen llpc_screen_init(const char *name);

LLPC_ScreenStatus llpc_screen_update(LLPC_Screen *screen, llpc_bool clear);
LLPC_ScreenStatus llpc_screen_event(LLPC_Screen *screen, const LLPC_ProcSignals sig);
LLPC_ScreenStatus llpc_screen_extevent(LLPC_Screen *screen, const LLPC_ProcSignals sig, const llpc_bool verbose);

llpc_bool llpc_screen_kill(LLPC_Screen *screen);
llpc_bool llpc_screen_destroy(LLPC_Screen *screen);

#define llpc_screen_event(_screen, _sig) llpc_screen_extevent(screen, sig, llpctrue);

LLPC_SCREEN_CLOSE

#endif  // INCLUDE_SCREEN_SCREEN_H_

