#include "ProjectileController.hpp"

ProjectileController::ProjectileController(GameObject* gameObject) : Component(gameObject) {}

void ProjectileController::onCollisionStart(PhysicsComponent* comp) {

}

void ProjectileController::onCollisionEnd(PhysicsComponent* comp) {

}

void ProjectileController::setSpeed(float speed) {
	ProjectileController::speed = speed;
}
float ProjectileController::getSpeed() {
	return speed;
}

void ProjectileController::setTarget(std::shared_ptr<EnemyController> enemy) {
	ProjectileController::enemy = enemy;
}

void ProjectileController::setStartingPos(glm::vec3 pos) {
	startingPos = pos;
}

void ProjectileController::setDamage(int damage) {
	ProjectileController::damage = damage;
}

void ProjectileController::update(float deltaTime) {
	if (!enemy || !enemy->getGameObject()) destinationReached = true;
	if (!destinationReached && movementTime < maxTimeAlive) {
		glm::vec3 enemyPos = enemy->getGameObject()->getPosition();
		glm::vec3 pos = gameObject->getPosition();

		float rad = glm::mix(0.0f, 3.14f, movementTime);
		glm::vec3 p = glm::mix(startingPos, enemyPos, movementTime);
		p.y += glm::sin(rad) * 10;
		gameObject->setPosition(p);
		float rotY = glm::degrees(std::atan2(enemyPos.z - pos.z, enemyPos.x - pos.x));
		float rotXZ = 45 - glm::degrees(rad / 2);
		gameObject->setRotation(glm::vec3(rotXZ, -rotY, rotXZ));
		if (glm::distance(gameObject->getPosition(), enemyPos) < 0.1f) {
			enemy->hurt(damage);
			destinationReached = true;
		}
		movementTime += deltaTime;
	}
	else {
		gameObject->name = gameObject->name + " (killed by ProjectileController::update)";
		gameObject->die();
	}

}

bool ProjectileController::isDestinationReached() {
	return destinationReached;
}