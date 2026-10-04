#ifndef INCLUDE_VECTOR_VECTOR_H_
#define INCLUDE_VECTOR_VECTOR_H_

#include <stddef.h>

#include "llpc/lib/types.h"

#ifdef __cplusplus
#	define LLPC_VECTOR_OPEN			extern "C" {
#	define LLPC_VECTOR_CLOSE			}
#else
#	define LLPC_VECTOR_OPEN
#	define LLPC_VECTOR_CLOSE
#endif

LLPC_VECTOR_OPEN

#ifndef LLPC_VECTOR_CAP_ADDITION
# 	define LLPC_VECTOR_CAP_ADDITION 		16
#endif

typedef struct LLPC_Vector
{
	void **vec;

	size_t cap;
	size_t size;

	llpc_bool dynamic;
	llpc_bool __initialized;
} LLPC_Vector;

typedef enum LLPC_VectorError
{
	LLPC_VEC_OK,
	LLPC_VEC_OutOfBounds,
	LLPC_VEC_ResizeError,
	LLPC_VEC_OOM,
	LLPC_VEC_NULL,
} LLPC_VectorError;

LLPC_Vector llpc_vector_init(const size_t cap, const llpc_bool dynamic);

LLPC_VectorError llpc_vector_deleteIndex(LLPC_Vector *vec, const size_t index, const llpc_bool fall);
LLPC_VectorError llpc_vector_replaceIndex(LLPC_Vector *vec, void *item, const size_t idx);
LLPC_VectorError llpc_vector_pushBack(LLPC_Vector *vec, void *item);
LLPC_VectorError llpc_vector_pushFront(LLPC_Vector *vec, void *item);

void llpc_vector_destroy(LLPC_Vector *vec);

LLPC_VECTOR_CLOSE

#endif  // INCLUDE_VECTOR_VECTOR_H_

