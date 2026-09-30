#include "llpc/lib/time/sleep.h"

#include <efi/efilib.h>

EFI_STATUS llpc_sleepMS(const UINT64 milliseconds)
{
    if (milliseconds == 0)
        return EFI_SUCCESS;

    return gBS->Stall(milliseconds * 1000);
}

