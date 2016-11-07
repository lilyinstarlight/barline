#include <stdio.h>
#include <string.h>

#include <alsa/asoundlib.h>

#include "vol.h"

int vol_percent(vol_t * vol) {
	snd_mixer_t * mixer;
	snd_mixer_selem_id_t * sid;
	snd_mixer_elem_t * element;
	long min, max;

	long volume;

	snd_mixer_open(&mixer, 1);

	snd_mixer_attach(mixer, vol->card);
	snd_mixer_selem_register(mixer, NULL, NULL);
	snd_mixer_load(mixer);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, vol->selem);

	element = snd_mixer_find_selem(mixer, sid);

	if (element == NULL)
		return -1;

	snd_mixer_selem_get_playback_volume_range(element, &min, &max);

	snd_mixer_selem_get_playback_volume(element, SND_MIXER_SCHN_FRONT_LEFT, &volume);

	snd_mixer_close(mixer);

	return (volume - min) * 100 / max;
}

bool vol_mute(vol_t * vol) {
	snd_mixer_t * mixer;
	snd_mixer_selem_id_t * sid;
	snd_mixer_elem_t * element;

	int on;

	snd_mixer_open(&mixer, 1);

	snd_mixer_attach(mixer, vol->card);
	snd_mixer_selem_register(mixer, NULL, NULL);
	snd_mixer_load(mixer);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, vol->selem);

	element = snd_mixer_find_selem(mixer, sid);

	if (element == NULL)
		return false;

	snd_mixer_selem_get_playback_switch(element, SND_MIXER_SCHN_FRONT_LEFT, &on);

	snd_mixer_close(mixer);

	return !on;
}

void vol_parse(const char * fmt, vol_t * vol) {
	int ret = sscanf(fmt, "%63s:%63s", vol->card, vol->selem);

	if (ret <= 0) {
		strncpy(vol->card, "default", sizeof(vol->card));
		strncpy(vol->selem, "Master", sizeof(vol->selem));
	}
	else if (ret == 1) {
		strncpy(vol->selem, "Master", sizeof(vol->selem));
	}
}

int vol_poll(vol_t * vol) {
	return -1;
}

size_t vol_format(vol_t * vol, char * buf, size_t size) {
	int percent = vol_percent(vol);
	bool mute = vol_mute(vol);

	size_t chars;

	if (mute)
		chars = snprintf(buf, size, "mute");
	else
		chars = snprintf(buf, size, "%d %%", percent);

	return chars;
}
