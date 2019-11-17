#pragma once
#include "ProjectileController.hpp"

class SplashProjectile : public ProjectileController {
public:
	explicit SplashProjectile(GameObject* gameObject, float speed, float damage, float damageRadius);

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

	float getDamageRadius();
private:
	float damageRadius;
};

