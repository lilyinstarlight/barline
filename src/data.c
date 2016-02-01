#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/wireless.h>

#include <alloca.h>

#include <alsa/asoundlib.h>

#include "data.h"

struct core {
	unsigned long long user, userlow, sys, idle, total;
};

struct core cores[CORE_MAX + 2];

int getcore(int core) {
	FILE * file;
	char ref[12];
	int ret;

	char name[12];
	unsigned long long user, userlow, sys, idle, total;

	int percent;

	if(core > CORE_MAX)
		return -1;

	file = fopen("/proc/stat", "r");
	if(file == NULL)
		return -1;

	if(core < 0)
		snprintf(ref, sizeof(ref), "cpu");
	else
		snprintf(ref, sizeof(ref), "cpu%d", core);

	do
		ret = fscanf(file, "%11s %llu %llu %llu %llu %*[^\n]", name, &user, &userlow, &sys, &idle);
	while(strcmp(name, ref) != 0 && ret != EOF);

	fclose(file);

	if(ret != 5)
		return -1;

	core++;

	total = (user - cores[core].user) + (userlow - cores[core].userlow) + (sys - cores[core].sys);
	percent = total * 100 / (total + idle - cores[core].idle);

	cores[core].user = user;
	cores[core].userlow = userlow;
	cores[core].sys = sys;
	cores[core].idle = idle;

	return percent;
}

int gettemp(const char * thermal) {
	FILE * file;
	char path[64];
	int ret;

	int temp;

	snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", thermal);

	file = fopen(path, "r");
	if(file == NULL)
		return -1;

	ret = fscanf(file, "%d", &temp);

	fclose(file);

	if(ret != 1)
		return -1;

	return temp / 1000;
}

int getmem() {
	FILE * file;
	int ret;

	int total, available;

	file = fopen("/proc/meminfo", "r");
	if(file == NULL)
		return -1;

	ret = fscanf(file, "MemTotal: %d kB\nMemFree: %*d kB\nMemAvailable: %d kB", &total, &available);

	fclose(file);

	if(ret != 2)
		return -1;

	return total - available;
}

int getvol(const char * card, const char * selement) {
	snd_mixer_t * mixer;
	snd_mixer_selem_id_t * sid;
	snd_mixer_elem_t * element;
	long min, max;

	long vol;
	int mute;

	snd_mixer_open(&mixer, 1);

	snd_mixer_attach(mixer, card);
	snd_mixer_selem_register(mixer, NULL, NULL);
	snd_mixer_load(mixer);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selement);

	element = snd_mixer_find_selem(mixer, sid);

	if(element == NULL)
		return -1;

	snd_mixer_selem_get_playback_volume_range(element, &min, &max);

	snd_mixer_selem_get_playback_volume(element, SND_MIXER_SCHN_FRONT_LEFT, &vol);
	snd_mixer_selem_get_playback_switch(element, SND_MIXER_SCHN_FRONT_LEFT, &mute);

	snd_mixer_close(mixer);

	return vol * 100 / max + !mute * 1000;
}

int getwlan(const char * iface, char * ssid, size_t size) {
	int iwsock;
	struct iwreq iwreq;
	int ret;

	iwsock = socket(AF_INET, SOCK_DGRAM, 0);

	if(iwsock < 0)
		return -2;

	strncpy(iwreq.ifr_name, iface, IFNAMSIZ);
	iwreq.u.essid.pointer = ssid;
	iwreq.u.essid.length = size - 1;

	ret = ioctl(iwsock, SIOCGIWESSID, &iwreq);

	if(ret < 0)
		ret = -1;
	else if(iwreq.u.essid.length == 0)
		ret = 1;
	else
		ret = 0;

	ssid[iwreq.u.essid.length] = '\0';

	close(iwsock);

	return ret;
}

int getbatt(const char * batt) {
	FILE * file;
	char path[64];
	int ret;

	char status[12];

	long full, now;
	int charging;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/energy_full", batt);

	file = fopen(path, "r");
	if(file == NULL)
		return - 1;

	ret = fscanf(file, "%ld", &full);

	fclose(file);

	if(ret != 1)
		return -1;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/energy_now", batt);

	file = fopen(path, "r");
	if(file == NULL)
		return -1;

	ret = fscanf(file, "%ld", &now);

	fclose(file);

	if(ret != 1)
		return -1;

	snprintf(path, sizeof(path), "/sys/class/power_supply/%s/status", batt);

	file = fopen(path, "r");
	if(file == NULL)
		return -1;

	ret = fscanf(file, "%11s", status);

	fclose(file);

	if(ret != 1)
		return -1;

	charging = strcmp(status, "Charging") == 0;

	return now * 100 / full + charging * 1000;
}

int gettime(char * buf, size_t size, const char * fmt, const char * tz) {
	time_t rtime;
	struct tm * itime;

	setenv("TZ", tz, 1);

	rtime = time(NULL);
	itime = localtime(&rtime);

	strftime(buf, size, fmt, itime);

	unsetenv("TZ");

	return 0;
}
