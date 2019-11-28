#pragma once
#include "Component.hpp"
#include "PhysicsComponent.hpp"

class EnemyController : public Component {
public:
	//explicit EnemyController(GameObject* gameObject);
	explicit EnemyController(GameObject* gameObject);

	void init(int health, float damage, int coinDrop, std::vector<glm::vec2> path);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	void update(float deltaTime) override;

	int getHealth();

	int getMaxHealth();

	int getDamage();

	int getCoinDrop();

	glm::vec2 getTilePosition();
private:
	glm::vec2 tilePosition;
	glm::vec2 windowPosition;
	glm::vec2 direction;
	std::shared_ptr<PhysicsComponent> phys;
	std::vector<glm::vec2> path;
	int initialHealth;
	int currentHealth;
	float damage;
	int coinDrop;
	float totalTime;
	float tileSize = 32;
	glm::vec2 pos;
	int width;

	void moveTo(glm::vec2 tilePos);
};