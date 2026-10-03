#include <efi/efierr.h>

#include "llpc/lib/alloc/alloc.h"
#include "llpc/lib/alloc/realloc.h"
#include "llpc/lib/alloc/alloc_impl.h"

#include "llpc/lib/io/output.h"
#include "llpc/lib/io/input.h"

#include "llpc/lib/globals.h"

EFI_INPUT_KEY llpc_io_extReadchar(void)
{
	EFI_INPUT_KEY key = {0};

	EFI_STATUS status = uefi_call_wrapper(
		LLPC_SystemTable->BootServices->WaitForEvent,
		3, 1,
		&LLPC_SystemTable->ConIn->WaitForKey, LLPC_NULL);

	if (EFI_ERROR(status))
		return key;

	status = LLPC_SystemTable->ConIn->ReadKeyStroke(
		LLPC_SystemTable->ConIn,
		&key
	);

	return key;
}

EFI_STATUS llpc_io_extReadline(CHAR16 **buff, const int cap, const CHAR16 del, const llpc_bool dynamic)
{
	if (!buff || !*buff)
		return EFI_INVALID_PARAMETER;

	const llpc_bool autoGrow = (cap == -1 && dynamic);

	if (!autoGrow && cap <= 1)
		return EFI_INVALID_PARAMETER;

	// `autoGrow` contract: caller must pre-allocate at least 8 CHAR16s.
	int len	   = 0;
	int curCap = autoGrow
		? 8
		: cap;

	while (1)
	{
		EFI_INPUT_KEY key = llpc_io_extReadchar();

		if (key.UnicodeChar == del)
		{
			(*buff)[len] = L'\0';
			llpc_io_echo(L"\r\n");

			return EFI_SUCCESS;
		}

		if (key.UnicodeChar == CHAR_BACKSPACE)
		{
			if (len > 0)
			{
				len--;
				(*buff)[len] = L'\0';

				llpc_io_echo(L"\b \b");
			}

			continue;
		}

		if (key.UnicodeChar == 0)
			continue;

		if (len >= curCap - 1)
		{
			if (!dynamic)
				continue;

			const UINTN reallocLen = (UINTN)curCap + 8;

			CHAR16 *tmpBuff = llpc_realloc(*buff, reallocLen * sizeof(CHAR16));

			if (!tmpBuff)
			{
				(*buff)[len] = L'\0';
				return EFI_OUT_OF_RESOURCES;
			}

			*buff = tmpBuff;
			curCap = (int)(llpc_allocSize(*buff) / sizeof(CHAR16));
		}

		// `key` is valid here...

		(*buff)[len++] = key.UnicodeChar;

		CHAR16 echo[2] = {
			key.UnicodeChar,
			L'\0'
		};

		llpc_io_echo(echo);
	}
}

