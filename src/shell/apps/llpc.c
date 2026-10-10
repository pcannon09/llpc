#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/fmt/fmtConvert.h"
#include "llpc/lib/globals.h"

#include "llpc/shell/shell.h"

#include "llpc/shell/apps/llpc.h"

#include "llpc/lib/alloc/free.h"
#include "llpc/lib/logging/logger.h"

#include "llpc/lib/string/string.h"
#include "llpc/predefines.h"
#include "llpc/shell/shell.h"

llpc_bool flag_runLine = llpcfalse;
char *flag_runLineCmd = LLPC_NULL;

LLPC_APP_DECL(llpc_app_llpc)
{
	LLPC_ProcessSector llpcSector = {0};

	const LLPC_ProcessSector procSector =
		llpc_proc_initSector(
				&llpc_processData,
				llpcSector.id,
				&llpcSector);

	if (procSector.initError != LLPC_PROC_ERR_OK)
		return LLPC_SHEC_ProcError;

	const LLPC_ParamGotInfo pHelp = llpc_argpar_get(&shell->gap, "llpc.help");
	const LLPC_ParamGotInfo pRunline = llpc_argpar_get(&shell->gap, "llpc.runline");

	if (pHelp.first)
	{
		char *help = llpc_argpar_help(&shell->gap, "llpc");

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

	else if (pRunline.first)
	{
		const size_t cmdIndex = pRunline.second + 1;

		if (cmdIndex >= (size_t)argc || argv[cmdIndex] == LLPC_NULL)
			return LLPC_SHEC_ParamError;

		flag_runLineCmd = argv[cmdIndex];
		flag_runLine = llpctrue;

		Print(L"%s\r\n", flag_runLineCmd);
	}

	llpc_app_llpc_impl(shell,
			argc, argv,
			procSector.pid);

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

	if (!flag_runLine)
		llpc_shell_loop(&subshell);

	else
	{
		const size_t cmdLen = llpc_strlen(flag_runLineCmd);

		if (cmdLen == 0 || cmdLen == (size_t)-1)
			return LLPC_SHEC_ParamError;

		CHAR16 *runlineCmdC16 = llpc_calloc(llpc_strlen(flag_runLineCmd), sizeof(*runlineCmdC16));
		llpc_toChar16(runlineCmdC16, flag_runLineCmd);

		const LLPC_ShellInfoStatus status =
			llpc_shell_runline(&subshell, LLPC_NULL, runlineCmdC16);

		if (status.first.code != 0)
			return status.first.code;
	}

	llpc_shell_destroy(&subshell);

	llpc_extlog(LLPC_LL_Log, "Closed shell with past PID of ",  llpcfalse);

	if (LLPC_LOG_LVLCHECK(LLPC_LL_Log))
		Print(L"`%u`\r\n", pid);

	llpc_proc_destroySector(&llpc_processData, pid);

	return LLPC_SHEC_OK;
}

