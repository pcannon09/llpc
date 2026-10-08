#include "llpc/shell/apps/echo.h"

#include "llpc/lib/string/parser.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/process/process.h"

#include <efi/efi.h>
#include <efi/efilib.h>

LLPC_APP_DECL(llpc_app_echo)
{
	LLPC_ProcessSector echoSector = {0};

	const LLPC_ProcessSector procSector =
		llpc_proc_initSector(
			&llpc_processData,
			echoSector.id,
			&echoSector);

	if (procSector.initError != LLPC_PROC_ERR_OK)
		return LLPC_SHEC_ProcError;

	if (llpc_argpar_get(&shell->gap, "echo.help"))
	{
		char *help = llpc_argpar_help(&shell->gap, "echo");

		if (help)
		{
			Print(L"%a\r\n", help);
			LLPC_FREE(help);
		}

		llpc_proc_destroySector(
				&llpc_processData,
				procSector.pid);

		return LLPC_SHEC_OK;
	}

	llpc_app_echo_impl(shell,
		argc, argv,
		procSector.pid);

	return LLPC_SHEC_OK;
}

LLPC_APP_DECL(llpc_app_echo_impl, const LLPC_PID pid)
{
	LLPC_UNUSED(shell);

	char *argvStr = llpc_strpar_argv2str(1, argc, argv);
	char *printer = llpc_strpar_getString(argvStr);

	Print(L"%a\r\n", printer);

	llpc_proc_destroySector(&llpc_processData, pid);

	// Free all function call variables
	LLPC_FREE(argvStr);
	LLPC_FREE(printer);

	return LLPC_SHEC_OK;
}

