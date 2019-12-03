#include "SpawnController.hpp"
#include "EnemyController.hpp"
#include "ModelLoader.hpp"

SpawnController::SpawnController(GameObject* gameObj) :
	Component(gameObj) {
	waitTimeAmount = 5;
}

void SpawnController::setGameObjects(std::vector<std::shared_ptr<GameObject>>* gameObjects) {
	this->gameObjects = gameObjects;
}
bool once = true;

void SpawnController::update(float deltaTime) {
	time += deltaTime;
	if (time >= waitTimeAmount/*once*/) {
		time = 0;
		createMonster();
		once = false;
	} 
}

void SpawnController::startSpawningCycle(std::vector<glm::vec2> path) {
	monsterPath = path;
}

void SpawnController::setWaitTime(float time) { waitTimeAmount = time; }

std::shared_ptr<EnemyController> SpawnController::createMonster() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();

	ModelLoader::loadModel(obj, "sphere", "sphere");
	//TODO change to starting position
	obj->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

	auto phys = obj->addComponent<PhysicsComponent>();
	phys->initCircle(b2_dynamicBody, 10 / 100, { 
		obj->getPosition().x / 100, obj->getPosition().y / 100 }, 1);

	auto enemyController = obj->addComponent<EnemyController>();
	enemyController->init(100, 20.0f, 5, monsterPath);

	gameObjects->push_back(obj);

	return enemyController;
}