#include "EnemyController.hpp"

EnemyController::EnemyController(GameObject* gameObject)
	: Component(gameObject) {
}

void EnemyController::init(int health, float damage, int coinDrop, std::vector<glm::vec2> path) {

	phys = gameObject->getComponent<PhysicsComponent>();
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

	totalTime += deltaTime;
	//TODO: intepolashion
	moveTo( glm::vec2(10,0));
}


void EnemyController::moveTo(glm::vec2 tilePos) {
	glm::vec2 offset{ tileSize / 2, tileSize / 2 };
	pos = offset + glm::vec2{ tilePos.x * tileSize,tilePos.y * tileSize };
	this->pos = pos;

	gameObject->setPosition(glm::vec3(pos.x, gameObject->getPosition().y, pos.y));

	glm::vec2 physPos = pos;
	physPos.x +=/* (width - 1) **/ 0.5f * tileSize;
	phys->moveTo(physPos / 100.0f);

}

int EnemyController::getHealth() { return currentHealth; }

int EnemyController::getMaxHealth() { return initialHealth;  }

int EnemyController::getCoinDrop() { return coinDrop;  }

int EnemyController::getDamage() { return damage; }

glm::vec2 EnemyController::getTilePosition() { return tilePosition; }