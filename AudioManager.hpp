#pragma once
#include "Component.hpp"

class AudioManager : public Component {
public:
	explicit AudioManager(GameObject* gameObject);

	void play();
	void playOnce();
	void playTimes(int amount);
private:

};

