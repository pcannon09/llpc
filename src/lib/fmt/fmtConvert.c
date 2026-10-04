#include <efi/x86_64/efibind.h>

#include "llpc/lib/fmt/fmtConvert.h"
#include "llpc/lib/types.h"

CHAR16 *llpc_toChar16(CHAR16 *dst, const char *src)
{
	if (!src || !dst)
		return LLPC_NULL;

	UINTN i = 0;

	while (src[i] != '\0')
	{
		dst[i] = (CHAR16)(unsigned char)src[i];
		i++;
	}

	dst[i] = L'\0';

	return dst;
}

char *llpc_toChar(char *dst, const CHAR16 *src)
{
	if (!src || !dst)
		return LLPC_NULL;

	UINTN i = 0;

	while (src[i] != '\0')
	{
		dst[i] = (char)src[i];
		i++;
	}

	dst[i] = '\0';

	return dst;
}

