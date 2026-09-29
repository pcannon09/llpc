#include "llpc/lib/globals.h"
#include "llpc/lib/types.h"

LLPC_AppData llpc_appData;

EFI_HANDLE LLPC_ImageHandle;
EFI_SYSTEM_TABLE *LLPC_SystemTable = LLPC_NULL;

LLPC_SystemError llpc_initialize(const LLPC_AppData appdata,
		EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	if (!ImageHandle || !SystemTable)
		return LLPC_SE_InitNull;

	LLPC_ImageHandle = ImageHandle;
	LLPC_SystemTable = SystemTable;

#ifdef _GNU_EFI
	InitializeLib(ImageHandle, SystemTable);
#endif

	// Application Data -- Sanity Checks
	if (appdata.version == 0)
		llpc_appData.version = LLPC_UEFI_VERSION(-1, -1);

	if (appdata.logLevel == LLPC_LL_None)
		llpc_appData.logLevel = LLPC_LL_Verbose;

	return LLPC_SE_OK;
}
