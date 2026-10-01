#include <efi/efi.h>
#include <efi/efilib.h>

#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/globals.h"

#include "llpc/lib/io/color.h"
#include "llpc/lib/io/output.h"

#include "llpc/lib/logging/logger.h"
#include "llpc/lib/string/string.h"

#include "llpc/lib/fmt/fmtConvert.h"

#include "llpc/lib/screen/screen.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	const LLPC_AppData mainApplication = {
		.name = "Low-Level Portable C",
		.shortName = "LLPC",
		.version = LLPC_UEFI_VERSION(0, 1),
		.logLevel = LLPC_LL_Verbose
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
		llpc_log(LLPC_LL_Fatal, 	"FATAL :-(   Failed to initialize");
		llpc_extlog(LLPC_LL_Fatal, 	"Error code: SYSERR-", llpcfalse);
		Print(L"%u", initStatus); // Get error code
		llpc_io_print(L"");

		return EFI_LOAD_ERROR;
	}

	llpc_log(LLPC_LL_Verbose, "Checking heap status...");

	if (llpc_allocHeap.size == 0)
	{
		llpc_log(LLPC_LL_Fatal, "Could not initialize heap; heap size is not valid");
		return EFI_BAD_BUFFER_SIZE;
	}

	llpc_log(LLPC_LL_Verbose, "Initializing screen...");

	LLPC_Screen screen = llpc_screen_init();
	llpc_screen_update(&screen, llpctrue);

	llpc_screen_destroy(&screen);

	return llpc_destroy();
}

