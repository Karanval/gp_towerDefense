#include "TowerController.hpp"
#include "BrickController.hpp"

TowerController::TowerController(GameObject* gameObject) : Component(gameObject) {
}

glm::vec3 TowerController::getPosition() {
	return position;
}

void TowerController::setPosition(glm::vec3 position) {
	TowerController::position = position;
}