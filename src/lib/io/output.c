#include <efi/efi.h>
#include <efi/efilib.h>
#include <efi/eficon.h>

#include <stdarg.h>

#include "llpc/lib/io/output.h"
#include "llpc/lib/io/io.h"

#include "llpc/lib/globals.h"
#include "llpc/lib/screen/screen.h"

LLPC_IOError llpc_io_echo(CHAR16 *message)
{
	const EFI_STATUS ret = uefi_call_wrapper(
		LLPC_SystemTable->ConOut->OutputString,
		2,
		LLPC_SystemTable->ConOut,
		message);

	return ret == EFI_SUCCESS
		? LLPC_IOE_OK
		: LLPC_IOE_OutputError;
}

LLPC_IOError llpc_io_print(CHAR16 *message)
{
	EFI_STATUS ret = uefi_call_wrapper(
		LLPC_SystemTable->ConOut->OutputString,
		2,
		LLPC_SystemTable->ConOut,
		message);

	if (llpc_io_echo(L"\r\n") != LLPC_IOE_OK)
		return LLPC_IOE_OutputError;

	return ret == EFI_SUCCESS
		? LLPC_IOE_OK
		: LLPC_IOE_OutputError;
}

LLPC_IOError llpc_io_action(const LLPC_OutputType action, void *p1, ...)
{
	switch (action)
	{
		case LLPC_OT_Clear:
			{
				LLPC_SystemTable->ConOut->ClearScreen(LLPC_SystemTable->ConOut);

				return LLPC_IOE_OK;
			}

		case LLPC_OT_CurGoto:
			{
				va_list arg;
				va_start(arg, p1);

				const unsigned int cx = va_arg(arg, unsigned int);
				const unsigned int cy = va_arg(arg, unsigned int);

				if (p1)
				{
					LLPC_Screen *screen = p1;

					screen->curX = cx;
					screen->curY = cy;
				}

				va_end(arg);

				LLPC_SystemTable->ConOut->SetCursorPosition(
						LLPC_SystemTable->ConOut,
						cx, cy);

				return LLPC_IOE_OK;
			}

		case LLPC_OT_SetCursorState:
			{
				va_list arg;
				va_start(arg, p1);

				const llpc_bool setVisible = va_arg(arg, llpc_bool);

				va_end(arg);

				LLPC_SystemTable->ConOut->EnableCursor(
						LLPC_SystemTable->ConOut,
						setVisible);

				return LLPC_IOE_OK;
			}

		default: return LLPC_IOE_NoCommand;
	}

	return LLPC_IOE_UNKNOWN;
}

