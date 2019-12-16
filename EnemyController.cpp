#include "EnemyController.hpp"
#include "TowerDefense.hpp"

EnemyController::EnemyController(GameObject* gameObject)
	: Component(gameObject) {
}

EnemyController::~EnemyController() {
}

void EnemyController::init(float health, int coinDrop, std::vector<glm::vec2>* path) {
	phys = gameObject->getComponent<PhysicsComponent>();
	initialHealth = health;
	currentHealth = initialHealth;
	this->coinDrop = coinDrop;
	this->path = path;
	waypointIndex = 0;
	moveToNextWaypoint();
	for (int i = 0; i < health; i++) {
		std::shared_ptr<GameObject> hp = TowerDefense::instance->createGameObject();
		std::stringstream ss;
		ss << gameObject->name << "_hp_" << i;
		hp->name = ss.str();
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
	std::string objectName = comp->getGameObject()->name;
	//std::cout << "#### Projectile collided with: " << comp->getGameObject()->name << "\n";
	if (objectName.find("Arrow") != std::string::npos) {
		std::shared_ptr<ProjectileController> pc = comp->getGameObject()->getComponent<ProjectileController>();
		hurt(pc->getDamage());
	}
		
}

void EnemyController::onCollisionEnd(PhysicsComponent* comp) {

}
void EnemyController::update(float deltaTime) {
	if (moving && !gameObject->isMarkedForDeath()) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextPosition = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextPosition - pos;
		glm::vec2 currentDirection = glm::normalize(heading);
		
		if ((direction.x!=0 && direction.x == -currentDirection.x) 
			|| (direction.y!=0 && direction.y == -currentDirection.y)) {
			glm::vec2 waypoint = path->at(waypointIndex) * tileSize;
			// Synchronize double values
			phys->setPosition(0.01f * waypoint);
			gameObject->setPosition(glm::vec3(waypoint.x, gameObject->getPosition().y, waypoint.y));
			moveToNextWaypoint();
		}

		// update hp-bar position and orientation
		glm::vec3 pos = gameObject->getPosition() +
						glm::vec3(0, gameObject->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax()[1].y, 0);
		glm::vec3 camPos = TowerDefense::instance->getCamera().getPosition();
		glm::vec3 rot = glm::vec3(45, 90 - glm::degrees(std::atan2(pos.z - camPos.z, pos.x - camPos.x)), -45);
		for (int i = 0; i < healthPoints.size(); i++) {
			std::shared_ptr<GameObject> hp = healthPoints[i];
			glm::vec3 offset = glm::vec3(hpSize.z, 5, hpSize.x * i - healthPoints.size() / 2 * hpSize.x);
			hp->setPosition(pos + offset);
			hp->setRotation(rot);
			if (currentHealth < i && hp->getComponent<MaterialComponent>()) {
				hp->getComponent<MaterialComponent>()->getMaterial()->setColor(missingHealthColor);
			}
		}
	}
}

void EnemyController::moveToNextWaypoint() {
	waypointIndex++;
	if (waypointIndex < path->size()) {
		pos = glm::vec2(gameObject->getPosition().x, gameObject->getPosition().z);
		glm::vec2 nextTilePos = tileSize * path->at(waypointIndex);
		glm::vec2 heading = nextTilePos - pos;
		direction = glm::normalize(heading);

		if (direction.x ==1 && direction.y == 0)
			gameObject->setRotation(glm::vec3(0, 0, 0));
		else if (direction.x == 0 && direction.y == 1)
			gameObject->setRotation(glm::vec3(0, -90, 0));
		else if (direction.x == 0 && direction.y == -1)
			gameObject->setRotation(glm::vec3(0, 90, 0));
		else if (direction.x == -1 && direction.y == 0)
			gameObject->setRotation(glm::vec3(0, -180, 0));

		phys->setLinearVelocity(speed * direction);
	}
	else {
		phys->setLinearVelocity(glm::vec2(0));
		moving = false;
		TowerDefense::instance->decrementHealthBy(1);
		for (int i = 0; i < healthPoints.size(); i++) {
			healthPoints[i]->die();
		}
		gameObject->die();
	}
}

float EnemyController::getHealth() { return currentHealth; }

float EnemyController::getMaxHealth() { return initialHealth; }

int EnemyController::getCoinDrop() { return coinDrop; }

void EnemyController::hurt(int hurtAmount) {
	currentHealth -= hurtAmount;

	if (currentHealth <= 0) {
		for (int i = 0; i < healthPoints.size(); i++) {
			healthPoints[i]->die();
		}
		if (!looted) {
			TowerDefense::instance->enemyHurt();
			TowerDefense::instance->incrementGoldBy(getCoinDrop());
			looted = true;
		}
		gameObject->die();
		for (auto healthObject : healthPoints)
			healthObject->die();
	}
}

void EnemyController::cleanComponent() {
	phys.reset();
	for (int i = 0; i < healthPoints.size(); i++)
		healthPoints[i].reset();

}