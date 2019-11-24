#include "EnemyController.hpp"

EnemyController::EnemyController(GameObject* gameObject)
	: Component(gameObject) {
}

void EnemyController::init(int health, float damage, int coinDrop, std::vector<glm::vec2> path) {
	initialHealth = health;
	currentHealth = initialHealth;
	this->damage = damage;
	this->coinDrop = coinDrop;
	this->path = path;
	tilePosition = path[0];
	path.erase(path.begin());
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {

}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}

void EnemyController::update(float deltaTime) {
	if (!path.empty()) {
		/*glm::vec2 nextTile = path.begin();
		direction = glm::normalize();*/
	} 
}

int EnemyController::getHealth() { return currentHealth; }

int EnemyController::getMaxHealth() { return initialHealth;  }

int EnemyController::getCoinDrop() { return coinDrop;  }

int EnemyController::getDamage() { return damage; }

glm::vec2 EnemyController::getTilePosition() { return tilePosition; }