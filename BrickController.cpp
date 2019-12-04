#include "BrickController.hpp"
#include "MeshComponent.hpp"

BrickController::BrickController(GameObject* gameObject) : Component(gameObject) {
}

void BrickController::update(float deltaTime) {
	if (towerController->isDirty()) {
		gameObject->setPosition(position + towerController->getPosition());
		markDirty();
	}
	if (isDirty()) {
		std::shared_ptr<MeshComponent> mesh = gameObject->getComponent<MeshComponent>();
		if (mesh) {
			bounds = mesh->getMesh()->getBoundsMinMax();
			bounds[0] += gameObject->getPosition();
			bounds[1] += gameObject->getPosition();
		}
		dirty = false;
	}
}

std::array<glm::vec3, 2> BrickController::getBounds() {
	return bounds;
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