#include "llpc/lib/globals.h"

#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/logging/logger.h"
#include "llpc/lib/process/process.h"
#include "llpc/lib/types.h"

#include <efi/efilib.h>

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

	// Initialize deps

	llpc_appData = appdata;

	const EFI_STATUS heapInitStatus = llpc_alloc_heapInit(LLPC_DEFAULT_NUM_PAGES);

	if (heapInitStatus != EFI_SUCCESS)
		return LLPC_SE_HeapInit;

	llpc_processData = llpc_proc_init();

	if (!llpc_processData.__initialized)
		return LLPC_SE_ProcInit;

	return LLPC_SE_OK;
}

EFI_STATUS llpc_destroy(void)
{
	llpc_log(LLPC_LL_Verbose, "Destroying all processes...");

	if (llpc_processData.__initialized)
	{
		const LLPC_ProcessError code =
			llpc_proc_destroy(&llpc_processData);

		if (code != LLPC_PROC_ERR_OK)
		{
			llpc_extlog(LLPC_LL_Error, 	"Failed to destroy *all* processes; Error code: SYSERR-", llpcfalse);
			Print(L"%u", code); // Get error code
		}
	}

	else llpc_log(LLPC_LL_Warning, "Process data is not initialized");

	llpc_log(LLPC_LL_Verbose, "Destroying heap...");

	const EFI_STATUS heapStatusCode = llpc_alloc_heapDestroy();

	if (heapStatusCode != EFI_SUCCESS)
	{
		llpc_extlog(LLPC_LL_Fatal, "Failed to destroy heap; Error code: EFI_STATUS-", llpcfalse);
		Print(L"%u", heapStatusCode); // Get error code
	}

	llpc_log(LLPC_LL_Verbose, "Goodbye! Exit.");

	return EFI_SUCCESS;
}

