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
	if (time >= waitTimeAmount) {
	//if (once) {
		time = 0;
		spawnEnemy();
		once = false;
	} 
}

void SpawnController::startSpawningCycle(std::vector<glm::vec2> path) {
	enemyPath = path;
}

void SpawnController::setWaitTime(float time) { waitTimeAmount = time; }

void SpawnController::spawnWave() {

}

std::shared_ptr<EnemyController> SpawnController::spawnEnemy() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();

	ModelLoader::loadModel(obj, "sphere", "sphere");
	glm::vec2 initialPosition = 32.0f*enemyPath[0];

	glm::vec3 objPos = glm::vec3(initialPosition.x, 0.0f, initialPosition.y);

	obj->setPosition(objPos);

	auto phys = obj->addComponent<PhysicsComponent>();
	phys->initCircle(b2_dynamicBody, 10 / 100, { 
		obj->getPosition().x / 100, obj->getPosition().z / 100 }, 1);

	auto enemyController = obj->addComponent<EnemyController>();
	enemyController->init(100, 20.0f, 5, &enemyPath);

	gameObjects->push_back(obj);

	return enemyController;
}