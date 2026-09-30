#include "llpc/lib/sound/controller.h"

void llpc_sound_audioSpeakerOn(const UINT32 freq)
{
	UINT16 div = (UINT16)(LLPC_SOUND_PIT_FREQ / freq);

	if (div == 0)
		div = 1;

	llpc_sound_outb(0x43, 0xB6);

	llpc_sound_outb(0x42, (UINT8)(div & 0xFF));
	llpc_sound_outb(0x42, (UINT8)(div >> 8));

	const UINT8 val = llpc_sound_inb(0x61);

	llpc_sound_outb(0x61, val | 0x03);
}

void llpc_sound_audioSpeakerOff(void)
{
	const UINT8 val = llpc_sound_inb(0x61);

	llpc_sound_outb(0x61, val & (UINT8)~0x03);
}

