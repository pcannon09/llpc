#include <efi/x86_64/efibind.h>

#include "llpc/lib/time/time.h"
#include "llpc/lib/globals.h"

LLPC_DTStatus llpc_time_get(LLPC_Time *time)
{
	EFI_TIME efiTime;
	EFI_STATUS status;

	if (LLPC_EP_FAIL_EXPR)
		return LLPC_DTS_FailDate;

	status = uefi_call_wrapper(gRT->GetTime, 2, &efiTime, LLPC_NULL);

	if (EFI_ERROR(status))
		return LLPC_DTS_EFI_StatusFailed;

	time->hour = efiTime.Hour;
	time->minute = efiTime.Minute;
	time->second = efiTime.Second;
	time->nanosecond = efiTime.Nanosecond;
	time->millisecond = efiTime.Nanosecond / __LLPC_MILLISECOND_SIZE;

	return LLPC_DTS_OK;
}

