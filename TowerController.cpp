#include "TowerController.hpp"
#include "BrickController.hpp"
#include "TowerDefense.hpp"
#include "ClickableComponent.hpp"
#include "Grid.hpp"

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
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (!built && snapping) {
			glm::ivec2 gridPos = field->getGridPos();
			if (TowerDefense::instance->getGrid()->allowsTowers(gridPos.x, gridPos.y)) {
				build();
			}
		}
		break;
	}
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
		field = clickable->getGameObject()->getComponent<FieldController>();
		if (field) {
			glm::vec3 pos = field->getGameObject()->getPosition();
			pos.y += clickable->getBounds()[1].y;
			setPosition(pos);
			glm::ivec2 gridPos = field->getGridPos();
			unbuildable = !TowerDefense::instance->getGrid()->allowsTowers(gridPos.x, gridPos.y);
			snapping = true;
		}
		else snapping = false;
	}
}

void TowerController::build() {
	std::shared_ptr<ClickableComponent> clickable = gameObject->getComponent<ClickableComponent>();
	if (clickable) clickable->setActive(true);
	built = true;
	snapping = false;
}

bool TowerController::isUnbuildable() {
	return unbuildable;
}

void TowerController::addUpgrade(std::string upgrade) {
	upgrades.push_back(upgrade);
}

std::vector<std::string>* TowerController::getUpgrades() {
	return &upgrades;
}

void TowerController::setCost(int cost) {
	TowerController::cost = cost;
}
int TowerController::getCost() {
	return cost;
}
void TowerController::setFirerate(float firerate) {
	TowerController::firerate = firerate;
}
float TowerController::getFirerate() {
	return firerate;
}
void TowerController::setRadius(float radius) {
	TowerController::radius = radius;
}
float TowerController::getRadius() {
	return radius;
}