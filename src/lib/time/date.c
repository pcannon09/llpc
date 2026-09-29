#include "llpc/lib/time/date.h"
#include "llpc/lib/globals.h"

LLPC_DTStatus llpc_date_get(LLPC_Date *date)
{
	EFI_TIME efiTime;
	EFI_STATUS status;

	if (LLPC_EP_FAIL_EXPR)
		return LLPC_DTS_FailDate;

	status = uefi_call_wrapper(gRT->GetTime, 2, &efiTime, LLPC_NULL);

	if (EFI_ERROR(status))
		return LLPC_DTS_EFI_StatusFailed;

	date->year = efiTime.Year;
	date->month = efiTime.Month;
	date->day = efiTime.Day;

	return LLPC_DTS_OK;
}
