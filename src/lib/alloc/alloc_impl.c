#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/types.h"

#include <efi/efidef.h>
#include <efi/x86_64/efibind.h>

EFI_STATUS llpc_alloc_heapInit(const UINTN pages)
{
	EFI_PHYSICAL_ADDRESS addr = 0;

	if (pages == 0)
		return EFI_INVALID_PARAMETER;

	// Get a valid (or invalid) address
	const EFI_STATUS status = uefi_call_wrapper(
			BS->AllocatePages, 4,
			AllocateAnyPages, EfiLoaderData,
			pages, &addr);

	// If invalid, return error
	if (EFI_ERROR(status))
		return status;

	// Setup heap allocator for current base
	llpc_allocHeap.base = (VOID*)(UINTN)addr;
	llpc_allocHeap.size = pages * LLPC_HEAP_PAGE_SIZE;

	// Setup heap allocator for first placement
	llpc_allocHeap.first = llpc_allocHeap.base;
	llpc_allocHeap.first->size =
		llpc_allocHeap.size - sizeof(LLPC_AllocBlock);

	llpc_allocHeap.first->free = llpctrue;
	llpc_allocHeap.first->next = LLPC_NULL;
	llpc_allocHeap.first->prev = LLPC_NULL;

	return EFI_SUCCESS;
}

void llpc_heapMerge(LLPC_AllocBlock *block)
{
	// Merge next block to main
	if (block->next && block->next->free)
	{
		LLPC_AllocBlock *next = block->next;

		block->size += sizeof(LLPC_AllocBlock) + next->size;
		block->next = next->next;

		if (block->next)
			block->next->prev = block;
	}

	// Merge previous block to main
	if (block->prev && block->prev->free)
	{
		LLPC_AllocBlock *prev = block->prev;

		block->size += sizeof(LLPC_AllocBlock) + block->size;
		block->next = prev->next;

		if (block->next)
			prev->next->prev = block;
	}
}

