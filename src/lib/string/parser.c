#include "llpc/lib/string/parser.h"
#include "llpc/lib/string/string.h"

#include "llpc/lib/alloc/calloc.h"

#include <efi/efi.h>
#include <efi/efilib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void llpc_strpar_skipWhitespaceC16(CHAR16 **ch16)
{
	while (LLPC_STR_ISWHITESPACE(**ch16))
		(*ch16)++;
}

void llpc_strpar_skipWhitespace(char **ch)
{
	while (LLPC_STR_ISWHITESPACE(**ch))
		(*ch)++;
}

char *llpc_strpar_argv2str(unsigned int start, unsigned int argc, char **argv)
{
	size_t len = 0;

	for (unsigned int i = start ; i < argc ; ++i)
		len += llpc_strlen(argv[i]) + 1;

	char *str = llpc_calloc(len, sizeof(char));

	if (!str)
		return LLPC_NULL;

	str[0] = '\0';

	for (unsigned int i = start ; i < argc ; ++i)
	{
		llpc_strcat(str, argv[i]);

		if (i + 1 < argc)
			llpc_strcat(str, " ");
	}

	return str; // caller has to free
}

char *llpc_strpar_getString(const char *full)
{
	if (full == NULL)
		return NULL;

	const size_t fullLen = llpc_strlen(full);
	char *returned = llpc_calloc(fullLen + 1, sizeof(char));

	if (returned == NULL)
		return NULL;

	llpc_bool inString = llpcfalse;
	size_t out = 0;

	for (size_t i = 0 ; i < fullLen ; ++i)
	{
		const char ch = full[i];

		if (ch == '"')
		{
			inString = !inString;
			continue;
		}

		if (!inString)
			continue;

		if (ch == '\\' && i + 1 < fullLen)
		{
			++i;
			returned[out++] = full[i];
			continue;
		}

		returned[out++] = ch;
	}

	returned[out] = '\0';

	return returned;
}

