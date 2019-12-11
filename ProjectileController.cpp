#include "ProjectileController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

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
		gameObject->setPosition(glm::mix(startingPos, enemyPos, movementTime));
		gameObject->setRotation(glm::degrees(atan((enemyPos.y - pos.y) / (enemyPos.x - pos.x))));
		if (glm::distance(gameObject->getPosition(), enemyPos) > 0.1f) destinationReached = true;
		movementTime += deltaTime;
	}
}

bool ProjectileController::isDestinationReached() {
	return destinationReached;
}