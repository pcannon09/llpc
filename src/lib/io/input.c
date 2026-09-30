#include <efi/efierr.h>

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

EFI_STATUS llpc_io_extReadline(CHAR16 *buff, const UINTN cap, const CHAR16 del)
{
	if (!buff || cap <= 1 || del == L'\0')
		return EFI_INVALID_PARAMETER;

	UINTN len = 0;

	while (1)
	{
		EFI_INPUT_KEY key = llpc_io_extReadchar();

		if (key.UnicodeChar == del)
		{
			buff[len] = L'\0';
			llpc_io_echo(L"\r\n");
			return EFI_SUCCESS;
		}

		if (key.UnicodeChar == CHAR_BACKSPACE)
		{
			if (len > 0)
			{
				len--;
				buff[len] = L'\0';

				llpc_io_echo(L"\b \b");
			}

			continue;
		}

		if (key.UnicodeChar == 0)
			continue;

		if (len >= cap - 1)
			continue;

		buff[len++] = key.UnicodeChar;

		CHAR16 echo[2] = {
			key.UnicodeChar,
			L'\0'
		};

		llpc_io_echo(echo);
	}
}

