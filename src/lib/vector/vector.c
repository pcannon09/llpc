#include "llpc/lib/vector/vector.h"

#include "llpc/lib/alloc/calloc.h"
#include "llpc/lib/alloc/free.h"
#include "llpc/lib/alloc/realloc.h"

#include "llpc/lib/string/string.h"
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
	if (!vec->__initialized)
		return LLPC_VEC_NULL;

	if (newCap <= vec->cap)
		return LLPC_VEC_ResizeError;

	void **tmpVec = llpc_realloc(
		vec->vec,
		newCap * sizeof(*vec->vec));

	if (!tmpVec)
		return LLPC_VEC_OOM;

	vec->vec = tmpVec;
	vec->cap = newCap;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_replaceIndex(LLPC_Vector *vec, void *item, const size_t idx)
{
	if (!vec->__initialized)
		return LLPC_VEC_NULL;

	if (idx >= vec->cap)
		return LLPC_VEC_OutOfBounds;

	vec->vec[idx] = item;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_pushFront(LLPC_Vector *vec, void *item)
{
	if (!vec || !vec->__initialized)
		return LLPC_VEC_NULL;

	if (vec->size >= vec->cap)
	{
		if (!vec->dynamic)
			return LLPC_VEC_OutOfBounds;

		const LLPC_VectorError error =
			llpc_vector_resize(
				vec,
				vec->cap + LLPC_VECTOR_CAP_ADDITION);

		if (error != LLPC_VEC_OK)
			return error;
	}

	for (size_t i = vec->size; i > 0; --i)
		vec->vec[i] = vec->vec[i - 1];

	vec->vec[0] = item;
	vec->size++;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_deleteIndex(LLPC_Vector *vec, const size_t index, const llpc_bool fall)
{
	if (!vec->__initialized)
		return LLPC_VEC_NULL;

	if (!fall)
	{
		vec->vec[index] = LLPC_NULL;

		return LLPC_VEC_OK;
	}

	// else:
	// NOT (expr) !!
	if (!(vec->size > index + 1))
		return LLPC_VEC_OutOfBounds;

	// Shift element by ONE
	for (size_t i = index ; i + 1 < vec->size ; ++i)
		vec->vec[i] = vec->vec[i + 1];

	vec->vec[vec->size - 1] = LLPC_NULL;
	vec->size--;

	return LLPC_VEC_OK;
}

LLPC_VectorError llpc_vector_pushBack(LLPC_Vector *vec, void *item)
{
	if (vec->size >= vec->cap && vec->dynamic)
	{
		const LLPC_VectorError error =
			llpc_vector_resize(vec, vec->cap + LLPC_VECTOR_CAP_ADDITION);

		if (error != LLPC_VEC_OK)
			return error;
	}

	const LLPC_VectorError error = 
		llpc_vector_replaceIndex(vec, item, vec->size);

	if (error != LLPC_VEC_OK)
		return error;

	vec->size++;

	return LLPC_VEC_OK;
}

void llpc_vector_destroy(LLPC_Vector *vec)
{
	llpc_nullify(vec->vec);

	vec->__initialized = llpcfalse;
	vec->cap = 0;
	vec->size = 0;
}

