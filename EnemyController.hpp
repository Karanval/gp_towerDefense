#pragma once
#include "Component.hpp"
class EnemyController : public Component {
public:
	//explicit EnemyController(GameObject* gameObject);
	explicit EnemyController(GameObject* gameObject, int health, float damage, int coinDrop, float speed);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	int getHealth();

	int getMaxHealth();

	int getDamage();

	int getCoinDrop();

	void setSpeed(float newSpeed);
private:
	std::shared_ptr<PhysicsComponent> phys;
	std::vector<glm::vec2> path;
	int initialHealth;
	int currentHealth;
	float damage;
	float speed;
	int coinDrop;
};