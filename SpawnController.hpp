#pragma once
#include "Component.hpp"
#include "EnemyController.hpp"

class SpawnController : Component {
public:
	explicit SpawnController(GameObject* gameObject);

	void startSpawningCycle(glm::vec2 initialPos, std::vector<glm::vec2> path);
private:
	//Timer
	bool waitTime;
	float time;

	EnemyController createMonster();
};

