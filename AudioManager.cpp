#include "AudioManager.hpp"
#include <iostream>
#include "SDL2-2.0.10\include\SDL.h"

AudioManager::AudioManager(GameObject* gameObject)
	: Component(gameObject) {
	int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
	if (result < 0)
	{
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(-1);
	}
}


void AudioManager::play(String filename) {
	Mix_Chunk* high = Mix_LoadWAV(filename);
}

void AudioManager::playOnce() {

}

void AudioManager::playTimes(int amount) {

}