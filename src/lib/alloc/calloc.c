#include <stdint.h>

#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/malloc.h"

extern void __llpc_memzero(void *ptr, size_t len);

void *llpc_calloc(const size_t nmemb, const size_t size)
{
	if (!nmemb || !size)
		return LLPC_NULL;

	// If this condition is met; overflow prevented
	if (nmemb > SIZE_MAX / size)
		return NULL;

	const size_t total = nmemb * size;
	void *ptr = llpc_malloc(total);

	if (!ptr)
		return NULL;

	// Fill `ptr` full of *zeros*
	__llpc_memzero(ptr, total);

	return ptr;
}

