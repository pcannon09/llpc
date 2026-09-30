#include "llpc/lib/io/color.h"
#include "llpc/lib/globals.h"

UINT8 llpc_io_originalFg = LLPC_IO_COLOR_WHITE;
UINT8 llpc_io_originalBg = LLPC_IO_COLOR_BG_BLACK;

void llpc_io_setColor(const UINT8 color)
{
	uefi_call_wrapper(
		LLPC_SystemTable->ConOut->SetAttribute,
		1,
		LLPC_SystemTable->ConOut,
		color);
}

llpc_bool llpc_io_isBgColor(const UINT8 color)
{
	if (color == EFI_BLACK)
		return llpcnone;

	return (color & 0xF0) != 0;
}

void llpc_io_resetColor(const LLPC_IO_ScreenTypes screenType)
{
	if (screenType & LLPC_IOST_BG)
		llpc_io_setColor(llpc_io_originalBg);

	if (screenType & LLPC_IOST_FG)
		llpc_io_setColor(llpc_io_originalFg);
}

