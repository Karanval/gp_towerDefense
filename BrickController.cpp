#include "BrickController.hpp"

BrickController::BrickController(GameObject* gameObject) : Component(gameObject) {
}

glm::vec3 BrickController::getPosition() {
	return towerController ? towerController->getPosition() + position : position;
}
glm::vec3 BrickController::getLocalPosition() {
	return position;
}
void BrickController::setLocalPosition(glm::vec3 position) {
	BrickController::position = position;
}
std::shared_ptr<TowerController> BrickController::getTowerController() {
	return towerController;
}
void BrickController::setTowerController(std::shared_ptr<TowerController> towerController) {
	BrickController::towerController = towerController;
}