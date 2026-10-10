#include "llpc/lib/string/parser.h"
#include "llpc/predefines.h"

#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"

#include "llpc/lib/fmt/fmtConvert.h"
#include "llpc/lib/io/input.h"
#include "llpc/lib/logging/logger.h"
#include "llpc/lib/string/string.h"

#include <efi/efidef.h>
#include <efi/efierr.h>
#include <efi/efilib.h>

#include "llpc/shell/shell.h"

#include "llpc/shell/apps/echo.h"
#include "llpc/shell/apps/exit.h"
#include "llpc/shell/apps/proc.h"
#include "llpc/shell/apps/read.h"
#include "llpc/shell/apps/sleep.h"
#include "llpc/shell/apps/sound.h"
#include "llpc/shell/apps/time.h"
#include "llpc/shell/apps/llpc.h"
#include "llpc/shell/modules/argpar.h"

LLPC_Shell llpc_shell_init(unsigned int argc, char **argv)
{
	LLPC_Shell shell = {0};

	shell.info.argc = argc;
	shell.info.argv = argv;
	shell.info.code = 0;

	shell.gap = llpc_argpar_init(
			"LLPC Shell Commands",
			"Essential commands for LLPC Shell");

	shell.commandsArrSize = 16;
	shell.commands = llpc_calloc(
			shell.commandsArrSize + 1,
			sizeof(LLPC_ShellCmdInfo));

	if (!shell.commands)
	{
		shell.initError = LLPC_SHEC_AllocError;
		return shell;
	}

	LLPC_ShellCmdInfo cmd_echo = {0};
	cmd_echo.command = "echo";
	cmd_echo.call = llpc_app_echo;

	LLPC_ArgPar *echoHelp = llpc_calloc(1, sizeof(*echoHelp)); // Parent
	LLPC_ArgPar *helpParam = llpc_calloc(1, sizeof(*helpParam));
	LLPC_ArgPar *newlineParam = llpc_calloc(1, sizeof(*newlineParam));

	if (!echoHelp || !helpParam || !newlineParam)
	{
		LLPC_FREE(echoHelp);
		LLPC_FREE(helpParam);
		LLPC_FREE(newlineParam);

		shell.initError = LLPC_SHEC_AllocError;
		return shell;
	}

	*echoHelp = llpc_argpar_sectorInit(
			&shell.gap,
			"echo",
			"Display a line of text as-is",
			llpcfalse, llpctrue);

	*helpParam = llpc_argpar_sectorInit(
			&shell.gap,
			"help",
			"Get this help",
			llpctrue, llpctrue);

	*newlineParam = llpc_argpar_sectorInit(
			&shell.gap,
			"newline",
			"Print new-line by default at the end of echoed message",
			llpctrue, llpctrue);

	llpc_vector_pushBack(&echoHelp->subparams, helpParam);
	llpc_vector_pushBack(&echoHelp->subparams, newlineParam);

	LLPC_ShellCmdInfo cmd_exit = {0};
	cmd_exit.command = "exit";

	LLPC_ShellCmdInfo cmd_proc = {0};
	cmd_proc.command = "proc";

	LLPC_ShellCmdInfo cmd_read = {0};
	cmd_read.command = "read";

	LLPC_ShellCmdInfo cmd_sleep = {0};
	cmd_sleep.command = "sleep";

	LLPC_ShellCmdInfo cmd_sound = {0};
	cmd_sound.command = "sound";

	LLPC_ShellCmdInfo cmd_time = {0};
	cmd_time.command = "time";

	LLPC_ShellCmdInfo cmd_llpc = {0};
	cmd_llpc.command = "llpc";
	cmd_llpc.call = llpc_app_llpc;

	LLPC_ArgPar *llpcHelp = llpc_calloc(1, sizeof(*echoHelp));
	LLPC_ArgPar *runLineParam = llpc_calloc(1, sizeof(*runLineParam));
	LLPC_ArgPar *llpcHelpParam = llpc_calloc(1, sizeof(*llpcHelpParam));

	if (!llpcHelp || !runLineParam)
	{
		LLPC_FREE(llpcHelp);
		LLPC_FREE(runLineParam);
		LLPC_FREE(runLineParam);

		shell.initError = LLPC_SHEC_AllocError;
		return shell;
	}

	*llpcHelp = llpc_argpar_sectorInit(
			&shell.gap,
			"llpc",
			"Open an interactive or a CLI shell session",
			llpctrue, llpctrue);

	*llpcHelpParam = llpc_argpar_sectorInit(
			&shell.gap,
			"help",
			"Get this help",
			llpctrue, llpctrue);

	*runLineParam = llpc_argpar_sectorInit(
			&shell.gap,
			"runline",
			"Execute a line of code inside a string",
			llpctrue, llpctrue);

	llpc_vector_pushBack(&llpcHelp->subparams, runLineParam);
	llpc_vector_pushBack(&llpcHelp->subparams, llpcHelpParam);

	shell.commands[0] = cmd_echo;
	shell.commands[1] = cmd_exit;
	shell.commands[2] = cmd_proc;
	shell.commands[3] = cmd_read;
	shell.commands[4] = cmd_sleep;
	shell.commands[5] = cmd_sound;
	shell.commands[6] = cmd_time;
	shell.commands[7] = cmd_llpc;

	shell.commands[8].__end = llpctrue;

	shell.help = llpc_argpar_help(&shell.gap, LLPC_NULL);

	if (!shell.help)
	{
		shell.initError = LLPC_SHEC_AllocError;
		return shell;
	}

	llpc_vector_pushBack(&shell.gap.args, echoHelp);
	llpc_vector_pushBack(&shell.gap.args, llpcHelp);

	shell.initError = LLPC_SHEC_OK;
	shell.__initialized = llpctrue;

	return shell;
}

LLPC_ShellInfoStatus llpc_shell_runline(
		LLPC_Shell *shell,
		LLPC_ShellInfo *shinfo,
		CHAR16 *commandBuff)
{
	llpc_strpar_skipWhitespaceC16(&commandBuff);

	LLPC_ShellInfoStatus retStatus = {0};
	
	if (shinfo)
		retStatus.first = *shinfo;

	if (!commandBuff && shinfo)
	{
		shinfo->code = LLPC_SHEC_SystemError;

		retStatus.first = *shinfo;
		return retStatus;
	}

	else if (!commandBuff && !shinfo)
		return retStatus;

	const size_t commandLen = llpc_strlen16(commandBuff);

	char *command = llpc_calloc(commandLen + 1, sizeof(char));

	if (!command)
	{
		shinfo->code = LLPC_SHEC_SystemError;

		retStatus.first = *shinfo;
		return retStatus;
	}

	llpc_toChar(command, commandBuff);

	char **commandArr = LLPC_NULL;
	const unsigned int splitted =
		llpc_split(command, ' ', &commandArr);

	if (splitted == 0 || !commandArr || !commandArr[0])
	{
		llpc_nullify(commandArr);
		llpc_nullify(command);

		retStatus.second = LLPC_SHRST_Continue;
		return retStatus;
	}

#if LLPC_DEV
		if (llpc_strcmp(commandArr[0], "dev-force-exit") ||
				llpc_strcmp(commandArr[0], "dfe"))
		{
			retStatus.second = LLPC_SHRST_Break;
			goto cleanup;
		}
#endif

	for (size_t i = 0; i < shell->commandsArrSize; ++i)
	{
		const LLPC_ShellCmdInfo cmd = shell->commands[i];

		if (cmd.__end)
			break;

		if (llpc_strcmp(commandArr[0], cmd.command))
		{
			if (cmd.call)
			{
				shell->gap.argc = splitted;
				shell->gap.argv = commandArr;

				cmd.call(shell, splitted, commandArr);

				shell->gap.argc = 0;
				shell->gap.argv = LLPC_NULL;
			}

			else
			{
				llpc_extlog(
						LLPC_LL_Error,
						"No such command call to: ",
						llpcfalse);

				CHAR16 cmdC16[llpc_strlen(cmd.command) + 1];

				llpc_toChar16(cmdC16, cmd.command);
				llpc_logecho(LLPC_LL_Error, cmdC16);
			}

			break;
		}
	}

cleanup:
	llpc_nullify(commandArr);
	llpc_nullify(command);

	return retStatus;
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

	if (!commandBuff)
	{
		shinfo.code = LLPC_SHEC_SystemError;
		return shinfo;
	}

	while (1)
	{
		shinfo.efiStatus =
			llpc_io_extReadline(
				&commandBuff,
				-1,
				CHAR_CARRIAGE_RETURN,
				llpctrue);

		if (shinfo.efiStatus != EFI_SUCCESS)
		{
			shinfo.code = LLPC_SHEC_EFI_Error;
			break;
		}

		LLPC_ShellInfoStatus status =
			llpc_shell_runline(shell, &shinfo, commandBuff);

		shinfo = status.first;

		if (status.second == LLPC_SHRST_Break)
			break;
		
		else if (status.second == LLPC_SHRST_Continue)
			continue;
	}

	llpc_nullify(commandBuff);

	return shinfo;
}

void llpc_shell_destroy(LLPC_Shell *shell)
{
	llpc_argpar_destroy(&shell->gap);
}

