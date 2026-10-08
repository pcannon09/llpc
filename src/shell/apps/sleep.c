#include "llpc/shell/apps/sleep.h"

LLPC_APP_DECL(llpc_app_sleep)
{
	LLPC_ProcessSector llpcSector = {0};

	const LLPC_ProcessSector procSector =
		llpc_proc_initSector(&llpc_processData, llpcSector.id, &llpcSector);

	if (procSector.initError != LLPC_PROC_ERR_OK)
		return LLPC_SHEC_ProcError;

	llpc_app_sleep_impl(shell, argc, argv, procSector.pid);

	return LLPC_SHEC_OK;
}

LLPC_APP_DECL(llpc_app_sleep_impl, const LLPC_PID pid)
{
	LLPC_UNUSED(shell);

	LLPC_UNUSED(argc);
	LLPC_UNUSED(argv);

	llpc_proc_destroySector(&llpc_processData, pid);

	return LLPC_SHEC_OK;
}

