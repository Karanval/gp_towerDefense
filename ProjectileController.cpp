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