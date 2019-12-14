#pragma once
#include "Component.hpp"
#include "PhysicsComponent.hpp"

// Base class for controlling the basic functions of Enemy Game Objects.
// An extension of a component.
class EnemyController : public Component {
public:
	//explicit EnemyController(GameObject* gameObject);
	explicit EnemyController(GameObject* gameObject);
	~EnemyController();

	void init(float health, float damage, int coinDrop, std::vector<glm::vec2>* path);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	void update(float deltaTime) override;

	float getHealth();

	float getMaxHealth();

	float getDamage();

	int getCoinDrop();

	void hurt(int hurtAmount);

private:
	glm::vec2 direction; 
	glm::vec2 pos;
	std::shared_ptr<PhysicsComponent> phys;
	std::vector<glm::vec2>* path;
	std::vector<std::shared_ptr<GameObject>> healthPoints;
	glm::vec3 hpSize;
	sre::Color healthColor = sre::Color(0, 1, 0, 1);
	sre::Color missingHealthColor = sre::Color(1, 0, 0, 1);
	int waypointIndex;
	int coinDrop;
	int width;
	float initialHealth;
	float currentHealth;
	float damage;
	float tileSize = 32.0f;
	float speed = 0.1f;
	bool moving = true;

	void moveToNextWaypoint();
};