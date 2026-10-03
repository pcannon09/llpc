#include "llpc/lib/alloc/alloc_impl.h"
#include "llpc/lib/types.h"

#include <efi/efidef.h>
#include <efi/x86_64/efibind.h>

EFI_STATUS llpc_alloc_heapInit(const UINTN pages)
{
	EFI_PHYSICAL_ADDRESS addr = 0;

	if (pages == 0)
		return EFI_INVALID_PARAMETER;

	const EFI_STATUS status = uefi_call_wrapper(
		BS->AllocatePages, 4,
		AllocateAnyPages,
		EfiLoaderData,
		pages,
		&addr
	);

	if (EFI_ERROR(status))
		return status;

	llpc_allocHeap.base = (VOID *)(UINTN)addr;
	llpc_allocHeap.size = pages * LLPC_HEAP_PAGE_SIZE;
	llpc_allocHeap.pages = pages;

	llpc_allocHeap.first = llpc_allocHeap.base;
	llpc_allocHeap.first->size =
		llpc_allocHeap.size - sizeof(LLPC_AllocBlock);

	llpc_allocHeap.first->free = llpctrue;
	llpc_allocHeap.first->next = LLPC_NULL;
	llpc_allocHeap.first->prev = LLPC_NULL;

	return EFI_SUCCESS;
}

EFI_STATUS llpc_alloc_heapDestroy(void)
{
	if (llpc_allocHeap.base == LLPC_NULL ||
		llpc_allocHeap.pages == 0)
		return EFI_SUCCESS;

	const EFI_STATUS status = uefi_call_wrapper(
		BS->FreePages, 2,
		(EFI_PHYSICAL_ADDRESS)(UINTN)llpc_allocHeap.base,
		llpc_allocHeap.pages
	);

	if (EFI_ERROR(status))
		return status;

	llpc_allocHeap.base = LLPC_NULL;
	llpc_allocHeap.size = 0;
	llpc_allocHeap.pages = 0;
	llpc_allocHeap.first = LLPC_NULL;

	return EFI_SUCCESS;
}

void llpc_heapMerge(LLPC_AllocBlock *block)
{
	// Merge with next free neighbour; `block` survives
	if (block->next && block->next->free)
	{
		LLPC_AllocBlock *next = block->next;

		block->size += sizeof(LLPC_AllocBlock) + next->size;
		block->next  = next->next;

		if (block->next)
			block->next->prev = block;
	}

	// Merge with previous free neighbour; `prev` survives, `block` is gone
	if (block->prev && block->prev->free)
	{
		LLPC_AllocBlock *prev = block->prev;

		prev->size += sizeof(LLPC_AllocBlock) + block->size;
		prev->next  = block->next;

		if (prev->next)
			prev->next->prev = prev;
	}
}

