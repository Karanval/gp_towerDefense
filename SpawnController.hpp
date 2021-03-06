#pragma once
#include "Component.hpp"
#include "EnemyController.hpp"
#include "ModelLoader.hpp"

// Class for handling spawning of enemies, and manipulating spawn waves.
class SpawnController : public Component {
public:
	explicit SpawnController(GameObject* gameObject);

	void setGameObjects(std::vector<std::shared_ptr<GameObject>>* gameObjects);

	void update(float deltaTime) override;

	void startSpawningCycle(std::vector<glm::vec2> path);

	void setWaitTime(float time);

	void setWaveAmount(int waveAmount);

	void setWaveTime(float waveTime);

	void addTenToEnemyHealth();

	void cleanComponent() override;

	std::string getName() override;

	std::shared_ptr<EnemyController> spawnEnemy();

private:
	float waveCurrentWait = 0;
	float enemyCurrentWait = 0;
	/* How much time between each enemy spawn*/
	float waitTimeAmount;
	/* How much time is between waves*/
	float waveTime;
	bool initialWait = false;
	/* How many enemies are spawned per wave*/
	int waveAmount;
	float enemyHealth = 20;
	int enemiesSpawn = 0;
	int increaseWaveAmount = 0;
	bool spawning = false;

	std::vector<std::shared_ptr<GameObject>>* gameObjects;
	std::vector<glm::vec2> enemyPath;

};

