#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"

enum TowerState {building, active};

class TowerController : Component {
public:
	explicit TowerController(GameObject* gameObject);
	
	void init(bool isStatic);
private:
	float fireSpeed;
	float radius;
	int cost;
	std::shared_ptr<ProjectileController> bullet;
};

