#pragma once
#include "Component.hpp"

class ProjectileController : public Component {
public:
	explicit ProjectileController(GameObject* gameObject, float speed, float damage);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	float getDamage();
private:
	std::shared_ptr<PhysicsComponent> phys;
	float speed;
	float damage;
};

