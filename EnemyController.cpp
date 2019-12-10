#include "EnemyController.hpp"

EnemyController::EnemyController(GameObject* gameObject)
	: Component(gameObject) {
}

void EnemyController::init(int health, float damage, int coinDrop, std::vector<glm::vec2>* path) {

	phys = gameObject->getComponent<PhysicsComponent>();
	initialHealth = health;
	currentHealth = initialHealth;
	this->damage = damage;
	this->coinDrop = coinDrop;
	this->path = path;
	waypointIndex = 0;
	moveToNextWaypoint();
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {
	// TODO Check if it is a bullet, if so get hurt
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}
void EnemyController::update(float deltaTime) {
	if (moving) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextPosition = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextPosition - pos;
		glm::vec2 currentDirection = glm::normalize(heading);

		if (direction.x == -currentDirection.x && direction.y == -currentDirection.y) {
			glm::vec2 waypoint = path->at(waypointIndex) *tileSize;
			// Synchronize double values
			// TODO think about adding the set position of the physics in gameobject?
			phys->setPosition(0.01f * waypoint);
			gameObject->setPosition(glm::vec3(waypoint.x, 0.0f, waypoint.y));
			moveToNextWaypoint();
		}
	}
}


void EnemyController::moveToNextWaypoint() {
	waypointIndex++;
	if (waypointIndex < path->size()) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextTilePos = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextTilePos - pos;
		direction = glm::normalize(heading);

		phys->setLinearVelocity(speed * direction);
	}
	else {
		phys->setLinearVelocity(glm::vec2(0));
		moving = false;
	}
}

int EnemyController::getHealth() { return currentHealth; }

int EnemyController::getMaxHealth() { return initialHealth;  }

int EnemyController::getCoinDrop() { return coinDrop;  }

int EnemyController::getDamage() { return damage; }

void EnemyController::hurt(float hurtAmount) {
	currentHealth -= hurtAmount;
	if (currentHealth <= 0) {
		//TODO die
	}
}
