#include "llpc/lib/string/string.h"
#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"

unsigned int llpc_split(const char *str, const char del, char ***out)
{
	if (!str || !out)
		return 0;

	*out = NULL;

	const unsigned int len = llpc_strlen(str);

	// An empty string is one empty token
	if (len == 0)
	{
		char **parts = llpc_calloc(2, sizeof(char *));

		if (!parts)
			return 0;

		parts[0] = llpc_calloc(1, sizeof(char));

		if (!parts[0])
		{
			llpc_nullify(parts);

			return 0;
		}

		parts[0][0] = '\0';
		parts[1] = NULL;

		*out = parts;

		return 1;
	}

	unsigned int count = 1;

	for (unsigned int i = 0 ; i < len ; ++i)
	{
		if (str[i] == del)
			count++;
	}

	char **parts =
		llpc_calloc(count + 1, sizeof(char *));

	if (!parts)
		return 0;

	unsigned int part = 0;
	unsigned int start = 0;

	for (unsigned int i = 0 ; i <= len ; ++i)
	{
		if (str[i] != del && str[i] != '\0')
			continue;

		const unsigned int partLen = i - start;

		parts[part] =
			llpc_calloc(partLen + 1, sizeof(char));

		if (!parts[part])
		{
			for (unsigned int j = 0 ; j < part ; ++j)
				llpc_nullify(parts[j]);

			llpc_nullify(parts);

			return 0;
		}

		if (partLen > 0)
			llpc_memcpy(parts[part], str + start, partLen);

		parts[part][partLen] = '\0';

		part++;
		start = i + 1;
	}

	parts[count] = NULL;

	*out = parts;

	return count;
}

unsigned int llpc_strlen16(const CHAR16 *message)
{
	if (!message)
		return 0;

	unsigned int count = 0;

	while (*message)
	{
		count++;
		message++;
	}

	return count;
}

unsigned int llpc_strlen(const char *message)
{
	if (!message)
		return 0;

	unsigned int count = 0;

	while (*message)
	{
		count++;
		message++;
	}

	return count;
}

void *llpc_memcpy(void *dest, const void *src, size_t len)
{
	char *d = (char*)dest;
	const char *s = (const char*)src;

	if (!d || !s)
		return NULL;

	for (size_t i = 0 ; i < len ; ++i)
		d[i] = s[i];

	return dest;
}

llpc_bool llpc_strncmp(const char *cmp1, const char *cmp2, unsigned int n)
{
	if (cmp1 == NULL || cmp2 == NULL)
		return (cmp1 == cmp2)
			? llpctrue
			: llpcfalse;

	for (unsigned int i = 0 ; i < n ; i++)
	{
		if (cmp1[i] != cmp2[i])
			return llpcfalse;

		// * `cmp1` and `cmp2` are both `'\0'` due to the
		// 	 previous check
		if (cmp1[i] == '\0')
			return llpctrue;
	}

	return llpctrue;
}

llpc_bool llpc_strcmp(const char *cmp1, const char *cmp2)
{
	if (cmp1 == NULL || cmp2 == NULL)
		return (cmp1 == cmp2)
			? llpctrue
			: llpcfalse;

	while (*cmp1 && *cmp2)
	{
		if (*cmp1 != *cmp2)
			return llpcfalse;

		cmp1++;
		cmp2++;
	}

	return (*cmp1 == '\0' && *cmp2 == '\0')
		? llpctrue
		: llpcfalse;
}

size_t llpc_strCountCh(const char *str, const char count)
{
	if (!str)
		return 0;

	const unsigned int strLen =
		llpc_strlen(str);

	size_t accum = 0;

	for (unsigned int i = 0 ; i < strLen ; ++i)
	{
		if (str[i] == count)
			accum++;
	}

	return accum;
}

// size_t llpc_strCount(const char *str, const char *count)
// {
// 	if (!str)
// 		return 0;
//
// 	const size_t strLen = llpc_strlen(str);
// 	size_t counter = 0;
//
// 	for (; counter < strLen ; ++counter)
// 	{
// 		if (str[counter] == count)
// 			counter++;
// 	}
//
// 	return 0;
// }

char *llpc_strdup(const char *src)
{
	char *dupped =
		llpc_calloc(llpc_strlen(src) + 1, sizeof(char*));

	llpc_memcpy(dupped, src, llpc_strlen(src) + 1);

	return dupped;
}

void *llpc_memmov(void *dst, const void *src, size_t n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;

	if (d == s || n == 0)
		return dst;

	if (d < s)
	{
		while (n--)
			*d++ = *s++;
	}

	else
	{
		d += n;
		s += n;

		while (n--)
			*--d = *--s;
	}

	return dst;
}

llpc_bool llpc_strAddIdx(char **str, const char *add, const size_t idx)
{
	if (!str || !*str || !add)
		return llpcfalse;

	const unsigned int strLen = llpc_strlen(*str);
	const unsigned int addLen = llpc_strlen(add);

	if (idx > strLen)
		return llpcfalse;

	char *total = llpc_calloc(strLen + addLen + 1, sizeof(char));

	if (!total)
		return llpcfalse;

	// Copy before insertion point
	for (unsigned int i = 0 ; i < idx ; ++i)
		total[i] = (*str)[i];

	// Copy inserted string
	for (size_t i = 0 ; i < addLen ; ++i)
		total[idx + i] = add[i];

	// Copy the rest of the original string
	for (size_t i = idx ; i < strLen ; ++i)
		total[addLen + i] = (*str)[i];

	total[strLen + addLen] = '\0';

	*str = total;

	return llpctrue;
}

void llpc_substr(char *str,
		unsigned int start,
		unsigned int end)
{
	const unsigned int len = end - start + 1;

	for (unsigned int i = 0 ; i < len ; i++)
		str[i] = str[start + i];

	str[len] = '\0';
}

unsigned int llpc_intstrlen(int n)
{
	unsigned int len = 0;

	if (n <= 0)
	{
		len++;
		n = -n;
	}

	while (n > 0)
	{
		len++;
		n /= 10;
	}

	return len;
}

void llpc_strreverse(char *str, const size_t nlen)
{
	for (size_t i = 0 ; i < nlen / 2 ; ++i)
	{
		const char tmpc = str[i];

		str[i] = str[nlen - 1 - i];
		str[nlen - 1 - i] = tmpc;
	}
}

