#pragma once
#include "Component.hpp"
#include "EnemyController.hpp"

class SpawnController : public Component {
public:
	explicit SpawnController(GameObject* gameObject);

	void setGameObjects(std::vector<std::shared_ptr<GameObject>>* gameObjects);

	void update(float deltaTime) override;

	void startSpawningCycle(std::vector<glm::vec2> path);

	void setWaitTime(float time);

	void setWaveAmount(int waveAmount);

	void setWaveTime(float waveTime);

	std::shared_ptr<EnemyController> spawnEnemy();

private:
	float waveCurrentWait = 0;
	float enemyCurrentWait = 0;
	/* How much time between each enemy spawn*/
	float waitTimeAmount;
	/* Hos much time is between waves*/
	float waveTime;
	/* How many enemies are spawned per wave*/
	int waveAmount;
	int enemiesSpawn = 0;
	bool spawning = false;


	std::vector<std::shared_ptr<GameObject>>* gameObjects;
	std::vector<glm::vec2> enemyPath;

};

