#ifndef INCLUDE_SHELL_SHELL_H_
#define INCLUDE_SHELL_SHELL_H_

#include "llpc/lib/process/process.h"

#include <efi/efi.h>
#include <efi/efidef.h>

#include <stdint.h>

#ifdef __cplusplus
#	define LLPC_SH_SHELL_OPEN			extern "C" {
#	define LLPC_SH_SHELL_CLOSE			}
#else
#	define LLPC_SH_SHELL_OPEN
#	define LLPC_SH_SHELL_CLOSE
#endif

LLPC_SH_SHELL_OPEN

#define LLPC_SHELL_MAX_PARAMS 					UINT8_MAX
#define LLPC_APP_PARAMS 						unsigned int argc, char **argv

#define LLPC_APP_DECL(_name, ...) 				LLPC_ShellErrorCode _name(LLPC_APP_PARAMS, ##__VA_ARGS__, ...)

typedef uint8_t 	LLPC_ShellRetCode;

typedef enum LLPC_ShellErrorCode
{
	LLPC_SHEC_OK 			= 0,
	LLPC_SHEC_ERROR 		= 1,
	LLPC_SHEC_SystemError 	= 2,
	LLPC_SHEC_EFI_Error,
	LLPC_SHEC_ProcError,

	__LLPC_SHEC_MAX 		= 255,
} LLPC_ShellErrorCode;

typedef struct LLPC_ShellInfo
{
	char **argv;
	unsigned int argc;

	EFI_STATUS efiStatus;

	LLPC_ShellRetCode code;
} LLPC_ShellInfo;

typedef struct LLPC_ShellCmdInfo
{
	char *command;

	// Function call
	LLPC_ShellErrorCode (*call)(unsigned int argc, char **argv, ...);

	llpc_bool __end;
} LLPC_ShellCmdInfo;

typedef struct LLPC_Shell
{
	LLPC_ShellErrorCode initError;

	LLPC_SignalSupportStatus obj;
	LLPC_ShellInfo info;

	unsigned int commandsArrSize;
	LLPC_ShellCmdInfo *commands;

	llpc_bool __initialized;
} LLPC_Shell;

LLPC_Shell llpc_shell_init(unsigned int argc, char **argv);

LLPC_ShellRetCode llpc_shell_exec(const char *procName,
		unsigned int argc, char **argv);
LLPC_ShellInfo llpc_shell_loop(LLPC_Shell *shell);

void llpc_shell_destroy(LLPC_Shell *shell);

LLPC_SH_SHELL_CLOSE

#endif  // INCLUDE_SHELL_SHELL_H_

