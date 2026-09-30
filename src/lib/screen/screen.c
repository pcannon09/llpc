#include "llpc/lib/screen/screen.h"
#include "llpc/lib/io/color.h"
#include "llpc/lib/io/output.h"
#include "llpc/lib/types.h"

LLPC_Screen llpc_screen_init(void)
{
	LLPC_Screen screen = {0};

	screen.curX = 0;
	screen.curY = 0;

	screen.screenColor = llpc_io_originalBg;
	screen.screenTextColor = llpc_io_originalFg;

	screen.__initialized = llpctrue;

	return screen;
}

LLPC_ScreenStatus llpc_screen_update(LLPC_Screen *screen, llpc_bool clear)
{
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

llpc_bool llpc_screen_destroy(LLPC_Screen *screen)
{
	screen->__initialized = llpcfalse;
	llpc_io_resetColor(LLPC_IOST_ALL);

	return llpctrue;
}

