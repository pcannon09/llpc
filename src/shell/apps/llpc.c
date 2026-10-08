#include "llpc/lib/globals.h"

#include "llpc/shell/apps/llpc.h"

#include "llpc/lib/logging/logger.h"
#include "llpc/predefines.h"

LLPC_APP_DECL(llpc_app_llpc)
{
	LLPC_ProcessSector llpcSector = {0};

	const LLPC_ProcessSector procSector =
		llpc_proc_initSector(&llpc_processData, llpcSector.id, &llpcSector);

	if (procSector.initError != LLPC_PROC_ERR_OK)
		return LLPC_SHEC_ProcError;

	llpc_app_llpc_impl(shell, argc, argv, procSector.pid);

	return LLPC_SHEC_OK;
}

LLPC_APP_DECL(llpc_app_llpc_impl, const LLPC_PID pid)
{
	LLPC_UNUSED(shell);

	llpc_extlog(LLPC_LL_Log, "Created shell with a PID of ", llpcfalse);

	if (LLPC_LOG_LVLCHECK(LLPC_LL_Log))
		Print(L"`%u`\r\n", pid);

	LLPC_Shell subshell = llpc_shell_init(argc, argv);
	llpc_proc_initSector(&llpc_processData, subshell.obj.id,
			&subshell.obj.sector);
	llpc_shell_loop(&subshell);
	llpc_shell_destroy(&subshell);

	llpc_extlog(LLPC_LL_Log, "Closed shell with past PID of ",  llpcfalse);

	if (LLPC_LOG_LVLCHECK(LLPC_LL_Log))
		Print(L"`%u`\r\n", pid);

	llpc_proc_destroySector(&llpc_processData, pid);

	return LLPC_SHEC_OK;
}

