#include "llpc/lib/globals.h"
#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/types.h"

LLPC_AppData llpc_appData;

EFI_HANDLE LLPC_ImageHandle;
EFI_SYSTEM_TABLE *LLPC_SystemTable = LLPC_NULL;

LLPC_SystemError llpc_initialize(LLPC_AppData appdata,
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
		appdata.version = LLPC_UEFI_VERSION(0, 0);

	if (appdata.logLevel == LLPC_LL_None)
		appdata.logLevel = LLPC_LL_Verbose;

	llpc_appData = appdata;

	const EFI_STATUS heapInitStatus = llpc_alloc_heapInit(16);

	if (heapInitStatus != EFI_SUCCESS)
		return LLPC_SE_HeapInit;

	return LLPC_SE_OK;
}
