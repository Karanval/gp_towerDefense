#include "AudioManager.hpp"
#include "SDL2-2.0.10\include\SDL.h"
#include "SDL2_mixer-2.0.4\include\SDL_mixer.h"

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

void AudioManager::playOnce(const char * filename) {
	Mix_Chunk* sound = Mix_LoadWAV(filename);;
	Mix_PlayChannel ( -1, sound, 0);
}

void AudioManager::playTimes(int amount) {

}

void AudioManager::cleanUP() {
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void AudioManager::pause() {
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}