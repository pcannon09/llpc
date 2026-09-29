#include "llpc/lib/logging/logger.h"

#include "llpc/lib/io/output.h"
#include "llpc/lib/string/string.h"
#include "llpc/lib/fmt/fmtConvert.h"

#include "llpc/lib/types.h"

#include "llpc/lib/time/date.h"
#include "llpc/lib/time/time.h"

#define __LLPC_timeBuff_TIME_LOG_EXAMPLE \
	"[ 00:00:00.0000 - 0000.00.00 ]  "

void llpc_logecho(CHAR16 *msg)
{
	uefi_call_wrapper(
		LLPC_SystemTable->ConOut->OutputString,
		2, LLPC_SystemTable->ConOut,
		msg);
}

void llpc_log(const LLPC_LogLevel level, const char *msg)
{
	if (level < llpc_appData.logLevel)
		return;

	CHAR16 timeBuff[llpc_strlen(__LLPC_timeBuff_TIME_LOG_EXAMPLE) + 1];

	LLPC_Time timeNow;
	llpc_time_get(&timeNow);

	LLPC_Date dateNow;
	llpc_date_get(&dateNow);

	CHAR16 messageC16[llpc_strlen(msg) + 1];
	llpc_toChar16(messageC16, msg);

	UnicodeSPrint(timeBuff, sizeof(timeBuff),
			L"[ %d:%d:%d.%d  %u.%u.%u ] ",
			// Time
			timeNow.hour,
			timeNow.minute,
			timeNow.second,
			timeNow.millisecond,

			// Date
			dateNow.year,
			dateNow.month,
			dateNow.day
	);

	llpc_logecho(timeBuff);
	llpc_logecho(messageC16);
	llpc_logecho(L"\n");
}

