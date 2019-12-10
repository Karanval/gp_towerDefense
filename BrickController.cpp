#include "BrickController.hpp"

BrickController::BrickController(GameObject* gameObject) : Component(gameObject) {
	BrickController::unbuildableMaterial = sre::Shader::getUnlit()->createMaterial();
	unbuildableMaterial->setColor(sre::Color(1, 0, 0));
}

void BrickController::update(float deltaTime) {
	if (towerController->isDirty()) {
		gameObject->setPosition(position + towerController->getGameObject()->getPosition());
		markDirty();
	}
	if (isDirty()) {
		if (towerController->isUnbuildable() && unbuildableMaterial) gameObject->getComponent<MaterialComponent>()->setMaterial(unbuildableMaterial);
		else if (defaultMaterial) gameObject->getComponent<MaterialComponent>()->setMaterial(defaultMaterial);
		else defaultMaterial = gameObject->getComponent<MaterialComponent>()->getMaterial();
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