#include "llpc/lib/vector/vector.h"

#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/realloc.h"

#include "llpc/lib/types.h"
#include <efi/efilib.h>

LLPC_Vector llpc_vector_init(const size_t cap, const llpc_bool dynamic)
{
	LLPC_Vector vec = {
		.cap = cap,
		.size = 0,
		.dynamic = dynamic,
		.__initialized = llpcfalse,
		.vec = LLPC_NULL
	};

	Print(L"calloc: cap=%u size=%u\r\n", cap, sizeof(*vec.vec));

	vec.vec = llpc_calloc(cap, sizeof(*vec.vec));

	Print(L"calloc result: %p\r\n", vec.vec);

	if (!vec.vec)
		return vec;

	vec.__initialized = llpctrue;

	return vec;
}

LLPC_VectorError llpc_vector_resize(LLPC_Vector *vec, const UINTN newCap)
{
	if (newCap < vec->cap)
		return LLPC_VEC_ResizeError;

	void **tmpVec = llpc_realloc(vec->vec, newCap);

	if (!tmpVec)
		return LLPC_VEC_OOM;

	else vec->vec = tmpVec;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_replaceIndex(LLPC_Vector *vec, void *item, const size_t idx)
{
	if (!vec->__initialized)
		return LLPC_VEC_NULL;

	if (idx > vec->cap)
		return LLPC_VEC_OutOfBounds;

	vec->vec[idx] = item;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_pushBack(LLPC_Vector *vec, void *item)
{
	const LLPC_VectorError error = 
		llpc_vector_replaceIndex(vec, item, vec->size + 1);

	if (error != LLPC_VEC_OK)
		return error;

	Print(L"Hello22\r\n");

	vec->size++;

	return LLPC_VEC_OK;
}

void llpc_vector_destroy(LLPC_Vector *vec)
{
	llpc_nullify(vec->vec);
	vec->__initialized = llpcfalse;
}

