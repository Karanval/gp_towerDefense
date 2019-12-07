#include "TowerController.hpp"
#include "BrickController.hpp"
#include "TowerDefense.hpp"
#include "ClickableComponent.hpp"
#include "FieldController.hpp"

TowerController::TowerController(GameObject* gameObject) : Component(gameObject) {
}

void TowerController::update(float deltaTime) {
	if (!built) snapToGrid();
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

void TowerController::snapToGrid() {
	std::shared_ptr<ClickableComponent> clickable = TowerDefense::instance->mouseToClickableObject();
	if (clickable) {
		std::shared_ptr<FieldController> field = clickable->getGameObject()->getComponent<FieldController>();
		setPosition(field->getGameObject()->getPosition());
	}
}

void TowerController::build() {
	std::shared_ptr<ClickableComponent> clickable = TowerDefense::instance->mouseToClickableObject();
	if (clickable) clickable->setActive(true);
}