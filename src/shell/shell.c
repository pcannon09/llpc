#include "llpc/lib/alloc/calloc.h"

#include "llpc/lib/io/input.h"
#include "llpc/lib/io/output.h"

#include <efi/efidef.h>
#include <efi/efierr.h>
#include <efi/efilib.h>

#include "llpc/shell/shell.h"

LLPC_Shell llpc_shell_init(unsigned int argc, char **argv)
{
	LLPC_Shell shell = {0};

	shell.info.argc = argc;
	shell.info.argv = argv;
	shell.info.code = 0;

	shell.initError = LLPC_SHEC_OK;
	shell.__initialized = llpctrue;

	return shell;
}

LLPC_ShellRetCode llpc_shell_exec(const char *procName,
		unsigned int argc, char **argv)
{

	return 0;
}

LLPC_ShellInfo llpc_shell_loop(LLPC_Shell *shell)
{
	LLPC_ShellInfo shinfo = {0};

	if (!shell || !shell->__initialized)
	{
		shinfo.code = LLPC_SHEC_SystemError;
		return shinfo;
	}

	CHAR16 *commandBuff = llpc_calloc(8, sizeof(CHAR16));

	while (1)
	{
		shinfo.efiStatus =
			llpc_io_extReadline(&commandBuff, -1, CHAR_CARRIAGE_RETURN, llpctrue);

		if (shinfo.efiStatus != EFI_SUCCESS)
		{
			shinfo.code = LLPC_SHEC_EFI_Error;
			return shinfo;
		}
	}

	return shinfo;
}

void llpc_shell_destroy(LLPC_Shell *shell)
{

}

