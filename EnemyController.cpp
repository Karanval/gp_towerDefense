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
	// todo: translate from x,y to x,z 
	// TODO change 5.0 to constant

	pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
	if (path.empty()) {
		direction = glm::vec2(0, 0);
		phys->setLinearVelocity(speed * direction);	
		//TODO: start atacking
	}
	else {
		glm::vec2 nextPosition = tileSize * tilePosition;
		glm::vec2 heading = nextPosition - pos;
		glm::vec2 currentDirection = glm::normalize(heading) ;
		//printf("nextPos %f %f #### Pos %f %f \n", nextPosition.x, nextPosition.y, pos.x, pos.y);
		//printf("heading %f %f \n", heading.x, heading.y);

		// Opposite directions -> Change direction
		if ((direction.x == 0 || direction.x == -currentDirection.x) 
			&& (direction.y == 0 || direction.y == -currentDirection.y)) {
			printf("changing direction");
			tilePosition = path[0];
			path.erase(path.begin());
			nextPosition = tileSize * tilePosition;
			heading = nextPosition - pos;
			currentDirection = glm::normalize(heading) ;
		}
		else {
			//printf("Dirx %f, %f, currentD: %f, %f \n", direction.x, direction.y, currentDirection.x, currentDirection.y);
		}
		if (direction.x != currentDirection.x || direction.y != currentDirection.y) {
			direction = currentDirection;

			phys->setLinearVelocity(speed * direction);
		}
	}
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