#include <efi/efi.h>
#include <efi/efilib.h>

#include "llpc/lib/globals.h"

#include "llpc/lib/io/color.h"
#include "llpc/lib/io/output.h"
#include "llpc/lib/string/string.h"
#include "llpc/lib/fmt/fmtConvert.h"

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	const LLPC_AppData mainApplication = {
		.name = "Low-Level Portable C",
		.shortName = "LLPC",
		.version = LLPC_UEFI_VERSION(0, 1),
		.logLevel = LLPC_LL_Verbose
	};

	if (llpc_initialize(mainApplication, ImageHandle, SystemTable) != LLPC_SE_OK)
		return EFI_LOAD_ERROR;

	// Shell init
	llpc_io_setColor(LLPC_IO_COLOR_BG_CYAN);
	llpc_io_echo(L"Initializing Shell: ");

	CHAR16 nameC16[llpc_strlen(llpc_appData.name) + 1];
	llpc_io_print(llpc_toChar16(nameC16, mainApplication.name));
	llpc_io_resetColor(LLPC_IOST_ALL);

	return EFI_SUCCESS;
}

