#pragma once
#include "Component.hpp"

class ProjectileController : public Component {
public:
	explicit ProjectileController(GameObject* gameObject);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	void setSpeed(float speed);
	float getSpeed();
private:
	std::shared_ptr<PhysicsComponent> phys;
	float speed;
	float damage;
};

