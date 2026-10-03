#include "llpc/lib/globals.h"
#include "llpc/lib/screen/screen.h"
#include "llpc/lib/io/color.h"
#include "llpc/lib/io/output.h"
#include "llpc/lib/logging/logger.h"
#include "llpc/lib/process/process.h"
#include "llpc/lib/types.h"

LLPC_Screen llpc_screen_init(const char *name)
{
	LLPC_Screen screen = {
		.obj.id = name
	};

	screen.curX = 0;
	screen.curY = 0;

	screen.screenColor = llpc_io_originalBg;
	screen.screenTextColor = llpc_io_originalFg;

	screen.__initialized = llpctrue;

	return screen;
}

LLPC_ScreenStatus llpc_screen_update(LLPC_Screen *screen, llpc_bool clear)
{
	if (LLPC_PROC_DENY_IF(screen))
		return LLPC_SSTAT_SigDenied;

	if (!screen->__initialized || !screen)
		return LLPC_SSTAT_NotInitialized;

	if (clear == llpcnone)
		clear = llpctrue;

	if (llpc_io_isBgColor(screen->screenColor))
	{
		llpc_io_setColor(
				(UINT8)EFI_TEXT_ATTR(screen->screenTextColor, screen->screenColor));
	}

	if (clear)
	{
		if (llpc_io_action(LLPC_OT_Clear, LLPC_NULL) != LLPC_IOE_OK)
			return LLPC_SSTAT_IOE_Error;
	}

	return LLPC_SSTAT_OK;
}

LLPC_ScreenStatus llpc_screen_extevent(LLPC_Screen *screen, const LLPC_ProcSignals sig, const llpc_bool verbose)
{
	if (!screen || !screen->__initialized)
		return LLPC_SSTAT_NotInitialized;

	screen->obj.currentSig = sig;

	switch (sig)
	{
		case __LLPC_PSIG_START: return LLPC_SSTAT_Invalid;
		case LLPC_PSIG_NONE: return LLPC_SSTAT_OK;
		case LLPC_PSIG_KILL: // Force kill, no matter what
			{
				if (verbose)
				{
					llpc_extlog(LLPC_LL_Log, "[KILL] PID ", llpcfalse);
					Print(L"%u - %a\r\n",
							screen->obj.sector.pid,
							screen->obj.sector.id);
				}

				llpc_screen_kill(screen);

				return LLPC_SSTAT_OK;
			}

		case LLPC_PSIG_BG:
			{
				if (verbose)
				{
					llpc_extlog(LLPC_LL_Log, "[BG] PID ", llpcfalse);
					Print(L"%u - %a\r\n",
							screen->obj.sector.pid,
							screen->obj.sector.id);
				}
			}

		case LLPC_PSIG_CLEAN:
			{
				if (verbose)
				{
					llpc_extlog(LLPC_LL_Log, "[PAUSE] PID ", llpcfalse);
					Print(L"%u - %a\r\n",
							screen->obj.sector.pid,
							screen->obj.sector.id);
				}

				if (!llpc_screen_destroy(screen))
					return LLPC_SSTAT_DestroyFail;

				return LLPC_SSTAT_OK;
			}
	}

	return LLPC_SSTAT_OK;
}

llpc_bool llpc_screen_kill(LLPC_Screen *screen)
{
	screen->__initialized = llpcfalse;

	return llpctrue;
}

llpc_bool llpc_screen_destroy(LLPC_Screen *screen)
{
	if (screen || screen->__initialized)
		return llpcfalse;

	screen->__initialized = llpcfalse;
	llpc_io_resetColor(LLPC_IOST_ALL);

	return llpctrue;
}

