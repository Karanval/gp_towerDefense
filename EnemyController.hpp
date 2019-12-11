#pragma once
#include "Component.hpp"
#include "PhysicsComponent.hpp"

class EnemyController : public Component {
public:
	//explicit EnemyController(GameObject* gameObject);
	explicit EnemyController(GameObject* gameObject);

	void init(int health, float damage, int coinDrop, std::vector<glm::vec2>* path);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	void update(float deltaTime) override;

	int getHealth();

	int getMaxHealth();

	int getDamage();

	int getCoinDrop();

	void hurt(float hurtAmount);

private:
	glm::vec2 direction; 
	glm::vec2 pos;
	std::shared_ptr<PhysicsComponent> phys;
	std::vector<glm::vec2>* path;
	int waypointIndex;
	int initialHealth;
	int currentHealth;
	int coinDrop;
	int width;
	float damage;
	float tileSize = 32.0f;
	float speed = 0.1f;
	bool moving = true;

	void moveToNextWaypoint();
};