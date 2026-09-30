#ifndef INCLUDE_SOUND_CONTROLLER_H_
#define INCLUDE_SOUND_CONTROLLER_H_

#include <efi/efi.h>

#ifdef __cplusplus
#	define LLPC_CONTROLLER_OPEN		extern "C" {
#	define LLPC_CONTROLLER_CLOSE		}
#else
#	define LLPC_CONTROLLER_OPEN
#	define LLPC_CONTROLLER_CLOSE
#endif

LLPC_CONTROLLER_OPEN

#define LLPC_SOUND_PIT_FREQ 	1193182U

extern void llpc_sound_outb(UINT16 port, UINT8 value);
extern UINT8 llpc_sound_inb(UINT16 port);

void llpc_sound_audioSpeakerOn(const UINT32 freq);
void llpc_sound_audioSpeakerOff(void);

LLPC_CONTROLLER_CLOSE

#endif  // INCLUDE_SOUND_CONTROLLER_H_

