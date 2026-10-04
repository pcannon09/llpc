#ifndef INCLUDE_STRING_STRING_COMMON_H_
#define INCLUDE_STRING_STRING_COMMON_H_

#include <efi/x86_64/efibind.h>
#include <stddef.h>

#include "llpc/lib/types.h"
#include "llpc/predefines.h"

#ifndef EFIAPI
# 	define EFIAPI
#endif

#ifdef __cplusplus
#	define LLPC_STRING_COMMON_OPEN			extern "C" {
#	define LLPC_STRING_COMMON_CLOSE			}
#else
#	define LLPC_STRING_COMMON_OPEN
#	define LLPC_STRING_COMMON_CLOSE
#endif

LLPC_STRING_COMMON_OPEN

LLPCAPI unsigned int llpc_strlen(const char *message);
LLPCAPI unsigned int llpc_strlen16(const CHAR16 *message);
LLPCAPI unsigned int llpc_intstrlen(int n);
LLPCAPI unsigned int llpc_findstr(const char *str, const char ch, const unsigned int pos);

LLPCAPI char *llpc_strcat(char *dest, const char *src);
LLPCAPI char *llpc_strdup(const char *src);

LLPCAPI void *llpc_memcpy(void *dest, const void *src, size_t len);

LLPCAPI void llpc_substr(char *str,
		unsigned int start,
		unsigned int end);
LLPCAPI void llpc_strreverse(char *str, const size_t nlen);

LLPCAPI llpc_bool llpc_strcmp(const char *cmp1, const char *cmp2);
LLPCAPI llpc_bool llpc_strncmp(const char *cmp1, const char *cmp2, const unsigned int n);
LLPCAPI llpc_bool llpc_strAddIdx(char **str, const char *add, const size_t idx);

LLPCAPI size_t llpc_strCountCh(const char *str, const char count);
LLPCAPI size_t llpc_strCount(const char *str, const char *count);

LLPCAPI void *llpc_memmov(void *dst, const void *src, size_t n);

LLPCAPI static inline llpc_bool llpc_strPushFront(char **str, const char *add)
{ return llpc_strAddIdx(str, add, 0); }

LLPCAPI static inline llpc_bool llpc_strPushBack(char **str, const char *add)
{ return llpc_strAddIdx(str, add, llpc_strlen(*str)); }

LLPCAPI unsigned int llpc_split(const char *str, const char delimiter, char ***out);

LLPC_STRING_COMMON_CLOSE

#endif  // INCLUDE_STRING_STRING_COMMON_H_

