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

	std::shared_ptr<EnemyController> createMonster();
private:
	//Timer
	float time = 0;
	float waitTimeAmount;
	std::vector<std::shared_ptr<GameObject>>* gameObjects;
	std::vector<glm::vec2> monsterPath;

};

