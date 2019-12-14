#include "EnemyController.hpp"
#include "TowerDefense.hpp"

EnemyController::EnemyController(GameObject* gameObject)
	: Component(gameObject) {
}

EnemyController::~EnemyController() {
	for (int i = 0; i < healthPoints.size(); i++) healthPoints[i]->die();
	//TowerDefense::instance->removeGameObject(healthPoints[i].get());
}

void EnemyController::init(float health, float damage, int coinDrop, std::vector<glm::vec2>* path) {
	phys = gameObject->getComponent<PhysicsComponent>();
	initialHealth = health;
	currentHealth = initialHealth;
	this->damage = damage;
	this->coinDrop = coinDrop;
	this->path = path;
	waypointIndex = 0;
	moveToNextWaypoint();
	for (int i = 0; i < health; i++) {
		std::shared_ptr<GameObject> hp = TowerDefense::instance->createGameObject();
		healthPoints.push_back(hp);
		TowerDefense::instance->getModelLoader()->loadModel(hp, "healthpoint", "healthpoint");
		if (i == 0) hpSize = hp->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax()[1] -
							 hp->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax()[0];

		std::shared_ptr<sre::Material> mat = sre::Shader::getUnlit()->createMaterial();
		mat->setColor(healthColor);
		healthPoints[i]->getComponent<MaterialComponent>()->setMaterial(mat);
	}
}

void EnemyController::onCollisionStart(PhysicsComponent* comp) {
	// TODO Check if it is a bullet, if so get hurt
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}
void EnemyController::update(float deltaTime) {
	if (moving) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextPosition = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextPosition - pos;
		glm::vec2 currentDirection = glm::normalize(heading);

		if (direction.x == -currentDirection.x && direction.y == -currentDirection.y) {
			glm::vec2 waypoint = path->at(waypointIndex) * tileSize;
			// Synchronize double values
			// TODO think about adding the set position of the physics in gameobject?
			phys->setPosition(0.01f * waypoint);
			gameObject->setPosition(glm::vec3(waypoint.x, gameObject->getPosition().y, waypoint.y));
			moveToNextWaypoint();
		}

		// update hp-bar position and orientation
		glm::vec3 pos = gameObject->getPosition();
		glm::vec3 camPos = TowerDefense::instance->getCamera().getPosition();
		glm::vec3 rot = glm::vec3(45, 90 - glm::degrees(std::atan2(pos.z - camPos.z, pos.x - camPos.x)), -45);
		for (int i = 0; i < healthPoints.size(); i++) {
			glm::vec3 offset = glm::vec3(hpSize.z, 5, hpSize.x * i - healthPoints.size() / 2 * hpSize.x);
			healthPoints[i]->setPosition(pos + offset);
			healthPoints[i]->setRotation(rot);
			if (currentHealth < i)
				healthPoints[i]->getComponent<MaterialComponent>()->getMaterial()->setColor(missingHealthColor);
		}
	}
}

void EnemyController::moveToNextWaypoint() {
	//TODO add Enemy rotate
	waypointIndex++;
	if (waypointIndex < path->size()) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextTilePos = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextTilePos - pos;
		direction = glm::normalize(heading);

		phys->setLinearVelocity(speed * direction);
	}
	else {
		phys->setLinearVelocity(glm::vec2(0));
		moving = false;
		TowerDefense::instance->decrementHealthBy(1);
		gameObject->die();
	}
}

float EnemyController::getHealth() { return currentHealth; }

float EnemyController::getMaxHealth() { return initialHealth; }

int EnemyController::getCoinDrop() { return coinDrop; }

float EnemyController::getDamage() { return damage; }

void EnemyController::hurt(int hurtAmount) {
	currentHealth -= hurtAmount;

	if (currentHealth <= 0) {
		gameObject->die();
	}
}
