#include "llpc/shell/apps/echo.h"

#include <efi/efi.h>
#include <efi/efilib.h>

LLPC_APP_DECL(llpc_app_echo)
{
	for (unsigned int i = 0 ; i < argc ; ++i)
	{
		const char *p = argv[i];

		Print(L"%a", p);
	}

	return LLPC_SHEC_OK;
}

