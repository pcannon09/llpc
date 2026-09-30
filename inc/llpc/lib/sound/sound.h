#ifndef INCLUDE_SOUND_SOUND_H_
#define INCLUDE_SOUND_SOUND_H_

#include <efi/efi.h>

#ifdef __cplusplus
#	define LLPC_SOUND_OPEN		extern "C" {
#	define LLPC_SOUND_CLOSE		}
#else
#	define LLPC_SOUND_OPEN
#	define LLPC_SOUND_CLOSE
#endif

LLPC_SOUND_OPEN

EFI_STATUS llpc_sound_beep(UINT32 frequency, const UINT32 durationMS);

LLPC_SOUND_CLOSE

#endif  // INCLUDE_SOUND_SOUND_H_
