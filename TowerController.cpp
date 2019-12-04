#include "TowerController.hpp"
#include "BrickController.hpp"

TowerController::TowerController(GameObject* gameObject) : Component(gameObject) {
}

void TowerController::update(float deltaTime) {
	
}

glm::vec3 TowerController::getPosition() {
	return position;
}

void TowerController::setPosition(glm::vec3 position) {
	TowerController::position = position;
	markDirty();
}

void TowerController::onMouse(SDL_Event& event) {

}

void TowerController::markDirty() {
	dirty = true;
}

bool TowerController::isDirty() {
	return dirty;
}