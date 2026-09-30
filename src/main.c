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
#include "llpc/lib/vector/vector.h"

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

	llpc_log(LLPC_LL_Verbose, "Initializing screen...");

	if (llpc_allocHeap.size == 0)
	{
		llpc_log(LLPC_LL_Fatal, "Could not initialize heap; heap size is not valid");
		return EFI_BAD_BUFFER_SIZE;
	}

	LLPC_Screen screen = llpc_screen_init();
	llpc_screen_update(&screen, llpctrue);

	// Shell init
	llpc_io_setColor(LLPC_IO_COLOR_BG_CYAN);
	llpc_io_echo(L"Initializing Shell: ");

	CHAR16 nameC16[llpc_strlen(llpc_appData.name) + 1];
	llpc_io_print(llpc_toChar16(nameC16, mainApplication.name));
	llpc_screen_update(&screen, llpcfalse);

	LLPC_Vector msgs = llpc_vector_init(2, llpctrue); // char*

	Print(L"CAP: %u\r\n", msgs.cap);
	Print(L"SIZE: %u\r\n", msgs.size);

	llpc_vector_pushBack(&msgs, "Hello");
	llpc_vector_pushBack(&msgs, "World");
	llpc_vector_pushBack(&msgs, "How");

	Print(L"\r\n%a\r\n\r\n", msgs.__initialized ? "true" : "false");

	for (UINT32 i = 0 ; i < msgs.size ; ++i)
	{
		Print(L"%u\r\n", msgs.vec[i]);
	}

	Print(L"==END==\r\n");
	Print(L"CAP: %u\r\n", msgs.cap);
	Print(L"SIZE: %u\r\n", msgs.size);

	// CHAR16 buff[99];
	// llpc_io_extReadline(buff, 99, CHAR_CARRIAGE_RETURN);

	llpc_screen_destroy(&screen);

	return EFI_SUCCESS;
}

