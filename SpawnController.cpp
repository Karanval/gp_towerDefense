#include "SpawnController.hpp"
#include "EnemyController.hpp"
#include "TowerDefense.hpp"

SpawnController::SpawnController(GameObject* gameObj) :
	Component(gameObj) {
	waitTimeAmount = 1;
	waveTime = 5;
	waveAmount = 4;
}

void SpawnController::setGameObjects(std::vector<std::shared_ptr<GameObject>>* gameObjects) {
	this->gameObjects = gameObjects;
}

void SpawnController::update(float deltaTime) {
	waveCurrentWait += deltaTime;
	enemyCurrentWait += deltaTime;
	spawning = enemiesSpawn < waveAmount;
	if (waveCurrentWait >= waveTime && enemiesSpawn < waveAmount) {
		if (enemyCurrentWait >= waitTimeAmount) {
			spawnEnemy();
			enemiesSpawn++;
			enemyCurrentWait = 0;
		}
	}
	else if (enemiesSpawn >= waveAmount) {
		waveCurrentWait = 0;
		enemiesSpawn = 0;
	}
}

void SpawnController::startSpawningCycle(std::vector<glm::vec2> path) {
	enemyPath = path;
}

void SpawnController::setWaitTime(float time) { waitTimeAmount = time; }

void SpawnController::setWaveAmount(int waveAmount) { this->waveAmount = waveAmount; }

void SpawnController::setWaveTime(float waveTime) { this->waveTime = waveTime; }

int enemies = 0;
std::shared_ptr<EnemyController> SpawnController::spawnEnemy() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	std::stringstream ss;
	ss << "Enemy_" << ++enemies;
	obj->name = ss.str();

	/*ModelLoader::loadModel(obj, "sphere", "sphere");
	glm::vec2 initialPosition = 32.0f*enemyPath[0];*/
	TowerDefense::instance->getModelLoader()->loadModel(obj, "cutout", "cutout", "lego_frankie.png");
	glm::vec2 initialPosition = 32.0f* enemyPath[0];

	glm::vec3 objPos = glm::vec3(initialPosition.x, 0.0f, initialPosition.y);

	obj->setPosition(objPos);

	auto phys = obj->addComponent<PhysicsComponent>();
	phys->initCircle(b2_dynamicBody, 10 / 100, 
	//phys->initBox(b2_dynamicBody, glm::vec2(16.0f, 20.0f), 
		{ obj->getPosition().x / 100, obj->getPosition().z / 100 }, 1);

	auto enemyController = obj->addComponent<EnemyController>();
	enemyController->init(20, 20.0f, 5, &enemyPath);

	gameObjects->push_back(obj);
	//waveTime = 5000;

	return enemyController;
}