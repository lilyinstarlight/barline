#include "vol.h"

int vol_percent(vol_t * vol) {
	snd_mixer_t * mixer;
	snd_mixer_selem_id_t * sid;
	snd_mixer_elem_t * element;
	long min, max;

	long vol;

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

	snd_mixer_selem_get_playback_volume(element, SND_MIXER_SCHN_FRONT_LEFT, &vol);

	snd_mixer_close(mixer);

	return vol * 100 / max;
}

bool vol_mute(vol_t * vol) {
	snd_mixer_t * mixer;
	snd_mixer_selem_id_t * sid;
	snd_mixer_elem_t * element;

	bool mute;

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

	snd_mixer_selem_get_playback_switch(element, SND_MIXER_SCHN_FRONT_LEFT, &mute);

	snd_mixer_close(mixer);

	return mute;
}

void vol_parse(const char * fmt, vol_t * vol) {
}

int vol_poll(const vol_t * vol) {
}

size_t vol_format(const vol_t * vol, char * buf, size_t size) {
}
