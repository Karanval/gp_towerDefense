#pragma once
#include "Component.hpp"
#include "SDL2_mixer-2.0.4\include\SDL_mixer.h"
#include <iostream>

static const char* MAIN_MUSIC = "../data/sounds/main.wav";
static const char* END_MUSIC = "../data/sounds/end.wav";
static const char* ENEMY_HURT = "../data/sounds/auch_enemy.wav";
static const char* SHOOT_ARROW = "../data/sounds/shoot_arrow.wav";
static const char* SHOOT_BOMB = "../data/sounds/shoot_bomb.wav";
static const char* SHOOT_1 = "../data/sounds/shoot_1.wav";
static const char* SHOOT_2 = "../data/sounds/shoot_2.wav";
static const char* DESTROY_TURRET = "../data/sounds/tower_fall.wav";
static const char* ATTACK_BASE = "../data/sounds/attack.wav";
static const char* UI_MENU = "../data/sounds/shoot_1.wav";
static const char* UI_CONFIRM = "../data/sounds/confirm.wav";
static const char* UI_ERROR = "../data/sounds/error.wav";

static const int MAX_VOLUME = MIX_MAX_VOLUME;

class AudioManager : public Component {
public:

	explicit AudioManager(GameObject* gameObject);

	void play(const char* filename);
	void playOnce(const char* filename);
	void playOnceWithVolume(const char* filename, int volume);
	void cleanUP();
	void pause();

private:

};

