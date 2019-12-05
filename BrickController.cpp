#include "BrickController.hpp"

BrickController::BrickController(GameObject* gameObject) : Component(gameObject) {
}

void BrickController::update(float deltaTime) {
	if (towerController->isDirty()) {
		gameObject->setPosition(position + towerController->getPosition());
		markDirty();
	}
	if (isDirty()) {
		// wash the dirty brick
		
		dirty = false;
	}
}

glm::vec3 BrickController::getPosition() {
	return gameObject->getPosition();
}
glm::vec3 BrickController::getLocalPosition() {
	return position;
}
void BrickController::setLocalPosition(glm::vec3 position) {
	BrickController::position = position;
	markDirty();
}
std::shared_ptr<TowerController> BrickController::getTowerController() {
	return towerController;
}
void BrickController::setTowerController(std::shared_ptr<TowerController> towerController) {
	BrickController::towerController = towerController;
}

void BrickController::markDirty() {
	dirty = true;
}

bool BrickController::isDirty() {
	return dirty;
}