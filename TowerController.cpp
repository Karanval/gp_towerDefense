#include "TowerController.hpp"
#include "BrickController.hpp"
#include "TowerDefense.hpp"
#include "ClickableComponent.hpp"
#include "Grid.hpp"

TowerController::TowerController(GameObject* gameObject) : Component(gameObject) {
}

void TowerController::update(float deltaTime) {
	if (!built) snapToGrid();
	else {
		if (firerate * (timeSinceBuilt - lastShotTime) > 1.0f) {
			std::shared_ptr<EnemyController> target = TowerDefense::instance->getClosestEnemy(gameObject->getPosition());
			if (target && glm::distance(target->getGameObject()->getPosition(), gameObject->getPosition()) <= radius) {
				shoot(target);
				lastShotTime = timeSinceBuilt;
			}
		}
		timeSinceBuilt += deltaTime;
	}
}

void TowerController::onMouse(SDL_Event& event) {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (!built && snapping) {
			glm::ivec2 gridPos = field->getGridPos();
			if (TowerDefense::instance->getGold() < cost) 
				TowerDefense::instance->displayMessage("Not enough gold!", ImVec4(1.0f, 0.8f, 0.05f, 1.0f));
			else if (TowerDefense::instance->getGrid()->allowsTowers(gridPos.x, gridPos.y)) {
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
			gameObject->setPosition(pos);
			glm::ivec2 gridPos = field->getGridPos();
			unbuildable = !TowerDefense::instance->getGrid()->allowsTowers(gridPos.x, gridPos.y) || TowerDefense::instance->getGold() < cost;
			snapping = true;
			markDirty();
		}
		else snapping = false;
	}
}

void TowerController::build() {
	std::shared_ptr<ClickableComponent> clickable = gameObject->getComponent<ClickableComponent>();
	TowerDefense::instance->decrementGoldBy(cost);
	if (clickable) clickable->setActive(true);
	built = true;
	snapping = false;
	dirty = false;
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
void TowerController::setDamage(int damage) {
	TowerController::damage = damage;
}
int TowerController::getDamage() {
	return damage;
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
void TowerController::setSpeed(float speed) {
	TowerController::speed = speed;
}
float TowerController::getSpeed() {
	return speed;
}
void TowerController::setProjectile(std::string projectile) {
	TowerController::projectile = projectile;
}
std::string TowerController::getProjectile() {
	return projectile;
}

void TowerController::shoot(std::shared_ptr<EnemyController> target) {
	std::shared_ptr<GameObject> projectileObj = TowerDefense::instance->createGameObject();
	std::shared_ptr<ProjectileController> projectileC = projectileObj->addComponent<ProjectileController>();
	TowerDefense::instance->getModelLoader()->loadModel(projectileObj, projectile, "lightgrey");
	projectileC->setStartingPos(gameObject->getComponent<ClickableComponent>()->getCenter());
	projectileC->setTarget(target);
	projectileC->setSpeed(speed);

}