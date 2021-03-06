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
			target = TowerDefense::instance->getClosestEnemy(gameObject->getPosition());
			if (target && glm::distance(target->getGameObject()->getPosition(), gameObject->getPosition()) <= radius) {
				std::shared_ptr<AudioManager> am = gameObject->getComponent<AudioManager>();
				if (am) {
					if (getCost() != 5)	am->playOnce(SHOOT_BOMB, 3);
					else am->playOnce(SHOOT_ARROW, 1);
				}
				shoot(target);
				lastShotTime = timeSinceBuilt;
			}
		}
		if (dirty) {
			bool takeShower = false;
			for (int i = 0; i < bricks.size(); i++) {
				if (bricks[i]->getComponent<BrickController>()->isDirty()) takeShower = true;
			}
			if (!takeShower) dirty = false;
		}
		timeSinceBuilt += deltaTime;
	}
	std::shared_ptr<ClickableComponent> clickable = gameObject->getComponent<ClickableComponent>();

	if (exploding && clickable) {
		if (bricks.size() == 0) {
			gameObject->die();
		}
		for (int i = 0; i < bricks.size(); i++) {
			auto brickObj = bricks[i];
			auto brickPos = brickObj->getPosition();
			glm::vec3 p = glm::mix(brickPos, glm::vec3(32, 32, 0), fallTime / 100);
			brickObj->setPosition(p);
			if (brickPos.y < clickable->getBounds()[0].y + 1) {
				clickable->setBounds({ glm::vec3(0,0,0),glm::vec3(0,0,0) });
				brickObj->die();
			}
		}

		fallTime += deltaTime;
	}
}

void TowerController::onMouse(SDL_Event& event) {
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.which == 0 && !built && snapping) {
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

bool TowerController::isBuilt() {
	return built;
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
	TowerDefense::instance->getModelLoader()->loadModel(projectileObj, projectile, projectile);
	projectileC->setStartingPos(gameObject->getComponent<ClickableComponent>()->getCenter());
	projectileC->setTarget(target);
	projectileC->setSpeed(speed);
	projectileC->setDamage(damage);
	projectileObj->name = "Arrow";

	std::shared_ptr<PhysicsComponent> phys = projectileObj->addComponent<PhysicsComponent>();
	phys->initBox(b2_dynamicBody, { 1 / PHYSICS_SCALE , 5 / PHYSICS_SCALE }, 
		{ projectileObj->getPosition().x / PHYSICS_SCALE, projectileObj->getPosition().z / PHYSICS_SCALE }, 1, true, -2);
	phys->isSensor();
	
}

void TowerController::explode() {
	if (exploding) return;
	exploding = true;
	TowerDefense::instance->incrementGoldBy(cost / 2);
	setFirerate(0.0f);
	auto towerPos = gameObject->getPosition();
	float physicsScale = TowerDefense::instance->physicsScale;
	for (int i = 0; i < bricks.size(); i++) {
		std::shared_ptr<GameObject> brickObj = bricks[i];
		auto pos = bricks[i]->getPosition();
		std::shared_ptr<PhysicsComponent> phys = brickObj->addComponent<PhysicsComponent>();
		phys->initBox(b2_dynamicBody, glm::vec2(0.1, 0.1) / physicsScale, glm::vec2(pos.x, pos.z) /physicsScale, 1, false, 3);
		if (i / 2 == 0) {
			phys->applyBlastImpulse(b2Vec2(1 / physicsScale, 1 / physicsScale) , b2Vec2(2 / physicsScale, 2 / physicsScale), 0.00000001);
		}
	}
}

void TowerController::destroy() {
	for (int i = bricks.size() - 1; i >= 0; i--) {
		bricks[i]->die();
	}
	gameObject->die();
}

bool TowerController::isExploding() {
	return exploding;
}

void TowerController::addBrick(std::shared_ptr<GameObject> brickObj) {
	bricks.push_back(brickObj);
}

std::vector<std::shared_ptr<GameObject>> TowerController::getBricks() {
	return bricks;
}

std::shared_ptr<EnemyController> TowerController::getTarget() {
	return target;
}

void TowerController::cleanComponent() {
	bullet.reset();
	field.reset();
	target.reset();
	phys.reset();
	for (int i = 0; i < bricks.size(); i++)
		bricks[i].reset();
	gameObject = nullptr;
}

std::string TowerController::getName() {
	return "Tower";
}