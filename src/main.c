#include <efi/efi.h>
#include <efi/efilib.h>

#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/globals.h"
#include "llpc/lib/asciiBanners.h"

#include "llpc/lib/io/color.h"
#include "llpc/lib/io/output.h"

#include "llpc/lib/logging/logger.h"
#include "llpc/lib/process/process.h"
#include "llpc/lib/sound/sound.h"
#include "llpc/lib/string/string.h"

#include "llpc/lib/fmt/fmtConvert.h"

#include "llpc/lib/screen/screen.h"
#include "llpc/shell/shell.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	const LLPC_AppData mainApplication = {
		.name = "Low-Level Portable C",
		.shortName = "LLPC",
		.version = LLPC_UEFI_VERSION(0, 1),
		.logLevel = LLPC_LL_Debug
	};

	const LLPC_SystemError initStatus =
		llpc_initialize(mainApplication, ImageHandle, SystemTable);

	llpc_extlog(LLPC_LL_Verbose, "", llpcfalse);
	llpc_io_setColor(LLPC_IO_COLOR_BG_CYAN);
	llpc_io_echo(L"Initializing Shell: ");

	CHAR16 nameC16[llpc_strlen(llpc_appData.name) + 1];
	llpc_io_print(llpc_toChar16(nameC16, mainApplication.name));
	llpc_io_resetColor(LLPC_IOST_ALL);

	if (initStatus != LLPC_SE_OK)
	{
		llpc_io_setColor(LLPC_IO_COLOR_WHITE | LLPC_IO_COLOR_BG_BLUE);
		llpc_io_action(LLPC_OT_Clear, LLPC_NULL);

		// Print the banner and set pos with color
		llpc_io_setColor(LLPC_IO_COLOR_WHITE | LLPC_IO_COLOR_BG_RED);
		llpc_io_action(LLPC_OT_CurGoto, LLPC_NULL, 1, 0);
		llpc_io_print(L"\n");

		if (LLPC_LOG_LVLCHECK(LLPC_LL_Fatal))
			Print(L"%s\n", llpc_ascii_fatalBanner());

		llpc_io_setColor(LLPC_IO_COLOR_WHITE | LLPC_IO_COLOR_BG_BLUE);
		llpc_io_action(LLPC_OT_CurGoto, LLPC_NULL, 1, 11);
		llpc_extlog(LLPC_LL_Fatal, 	"Error code: SYSERR-", llpcfalse);
		Print(L"%u", initStatus); // Get error code
		llpc_io_print(L"");

		llpc_io_resetColor(LLPC_IOST_ALL);

		return EFI_LOAD_ERROR;
	}

	llpc_log(LLPC_LL_Verbose, "Checking heap status...");

	if (llpc_allocHeap.size == 0)
	{
		llpc_log(LLPC_LL_Fatal, "Could not initialize heap; heap size is not valid");
		return EFI_BAD_BUFFER_SIZE;
	}

	llpc_log(LLPC_LL_Verbose, "Initializing screen...");

	LLPC_Screen screen = llpc_screen_init("main-screen");
	llpc_proc_initSector(&llpc_processData, screen.obj.id, &screen,
			&screen.obj.sector);
	llpc_screen_update(&screen, LLPC_DEF_CLS);

	LLPC_Shell shell = llpc_shell_init(0, LLPC_NULL);
	llpc_proc_initSector(&llpc_processData, shell.obj.id, &shell,
			&shell.obj.sector);

	llpc_shell_loop(&shell);

	llpc_shell_destroy(&shell);

	return llpc_destroy();
}

