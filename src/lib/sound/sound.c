#include "llpc/lib/sound/sound.h"
#include "llpc/lib/sound/controller.h"
#include "llpc/lib/time/sleep.h"

EFI_STATUS llpc_sound_beep(UINT32 frequency, const UINT32 durationMS)
{
	if (frequency == 0 || durationMS == 0)
		return EFI_INVALID_PARAMETER;

	if (frequency > LLPC_SOUND_PIT_FREQ)
		return EFI_INVALID_PARAMETER;

	llpc_sound_audioSpeakerOn(frequency);
	llpc_sleepMS(durationMS);
	llpc_sound_audioSpeakerOff();

	return EFI_SUCCESS;
}

