#pragma once
#include "Component.hpp"
#include "EnemyController.hpp"

// Class for handling projectile shooting from towers.
class ProjectileController : public Component {
public:
	explicit ProjectileController(GameObject* gameObject);

	void update(float deltaTime) override;
	void onCollisionStart(PhysicsComponent* comp) override;
	void onCollisionEnd(PhysicsComponent* comp) override;
	// Travelling speed
	void setSpeed(float speed);
	float getSpeed();
	void setTarget(std::shared_ptr<EnemyController> enemy); // enemy to chase
	// Damage dealt on impact
	void setDamage(int damage);
	float getDamage();
	// Starting position, this is normally the tower's position where the projectile was spawned
	void setStartingPos(glm::vec3 pos);
	bool isDestinationReached(); // true when target is either dead or reached
	void cleanComponent() override;


private:
	std::shared_ptr<PhysicsComponent> phys;
	float speed;
	float damage = 15;
	float movementTime = 0.0f;
	float maxTimeAlive = 2;
	std::shared_ptr<EnemyController> enemy;
	glm::vec3 startingPos;
	bool destinationReached = false;
	
};

