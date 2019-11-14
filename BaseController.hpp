#pragma once
#include "Component.hpp"

class BaseController : Component {
public:
	explicit BaseController(GameObject* gameObject);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;
private:
	std::shared_ptr<PhysicsComponent> phys;
	float hp;

};

