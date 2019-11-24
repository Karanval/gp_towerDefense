#include "SpawnController.hpp"
#include "EnemyController.hpp"
#include "ModelLoader.hpp"

SpawnController::SpawnController(GameObject* gameObj) :
	Component(gameObj) {
	waitTimeAmount = 5;
}

void SpawnController::setGameObjects(std::vector<std::shared_ptr<GameObject>> gameObjects) {
	this->gameObjects = gameObjects;
}

void SpawnController::update(float deltaTime) {
	time += deltaTime;
	printf("TiMe %f", time);
	if (time >= waitTimeAmount) {
		time = 0;
		createMonster();
	} 
}

void SpawnController::startSpawningCycle(std::vector<glm::vec2> path) {
	monsterPath = path;
}

void SpawnController::setWaitTime(float time) { waitTimeAmount = time; }

std::shared_ptr<EnemyController> SpawnController::createMonster() {
	printf("REached creating monster");
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	auto enemyController = obj->addComponent<EnemyController>();
	enemyController->init(100, 20.0f, 5, monsterPath);
	gameObjects.push_back(obj);

	ModelLoader::loadModel(obj, "sphere", "sphere");
	obj->setPosition(glm::vec3(5, 5.0f, 10));

	return enemyController;
}