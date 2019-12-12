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

void ProjectileController::update(float deltaTime) {
	if (enemy) {
		glm::vec3 enemyPos = enemy->getGameObject()->getPosition();
		glm::vec3 pos = gameObject->getPosition();

		/*float t = fmod(movementTime, 1.0f);
		float b0 = (1 - t) * (1 - t);
		float b1 = 2 * (1 - t) * t;
		float b2 = t * t;
		glm::vec3 p = b0 * startingPos + b1 * pos + b2 * enemyPos;*/


		float rad = glm::mix(0.0f, 3.14f, movementTime);
		glm::vec3 p = glm::mix(startingPos, enemyPos, movementTime);
		p.y += glm::sin(rad) * 10;
		gameObject->setPosition(p);
		float rotY = glm::degrees(std::atan2(enemyPos.z - pos.z, enemyPos.x - pos.x));
		float rotXZ = 45 - glm::degrees(rad / 2);
		gameObject->setRotation(glm::vec3(rotXZ, -rotY, rotXZ));
		if (glm::distance(gameObject->getPosition(), enemyPos) > 0.1f) {
			destinationReached = true;
			enemy->hurt(damage);
		}
		movementTime += deltaTime;
	}
}

bool ProjectileController::isDestinationReached() {
	return destinationReached;
}