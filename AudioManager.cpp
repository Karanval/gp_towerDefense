#include "AudioManager.hpp"
#include "SDL2_mixer-2.0.4\include\SDL_mixer.h"
#include "TowerDefense.hpp"

Mix_Music* music = NULL;

AudioManager::AudioManager(GameObject* gameObject)
	: Component(gameObject) {
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(-1);
	}
}



void AudioManager::play(const char * filename) {
	music = Mix_LoadMUS(filename);
	if (music == NULL)
		printf("Music was NOT loaded");
	Mix_PlayMusic(music, -1);
}

void AudioManager::playOnce(const char * filename, float lifespan) {
	Mix_Chunk* sound = Mix_LoadWAV(filename);;
	if (sound == NULL)
		printf("Sound was NOT loaded");
	Mix_PlayChannel ( -1, sound, 0);
	registerSound({ sound, TowerDefense::instance->getGameTime() + lifespan });
}

void AudioManager::playOnceWithVolume(const char * filename, int volume, float lifespan) {
	Mix_Chunk* sound = Mix_LoadWAV(filename);;
	if (sound == NULL)
		printf("Sound was NOT loaded");
	Mix_VolumeChunk(sound, volume);
	Mix_PlayChannel ( -1, sound, 0);
	registerSound({ sound, TowerDefense::instance->getGameTime() + lifespan });
}

void AudioManager::cleanUP() {
	for (auto it = soundregistry.begin(); it != soundregistry.end(); it = soundregistry.erase(it))
		Mix_FreeChunk(it->sound);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void AudioManager::pause() {
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

void AudioManager::registerSound(audio sound) {
	// before registering, free up finished sounds
	// NOTE: this step cannot be done in an update-call, as deletion would interfer with insertion
	// NOTE: this approach will only clean sounds whenever a new sound is registered, or when the cleanUP function is called.
	for (auto it = soundregistry.begin(); it != soundregistry.end(); it = soundregistry.erase(it)) {
		if (it->deletionTime < TowerDefense::instance->getGameTime()) Mix_FreeChunk(it->sound); // deletion time past, free up space
		else break; // list is ordered, if current shouldn't be deleted, then following shouldn't either
	}

	bool added = false;
	for (auto it = soundregistry.begin(); it != soundregistry.end(); it++) {
		if (sound.deletionTime < it->deletionTime) {
			soundregistry.insert(it, sound);
			added = true;
			break;
		}
	}
	if (!added) soundregistry.push_back(sound);
}