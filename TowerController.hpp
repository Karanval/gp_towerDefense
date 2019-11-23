#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"
#include "BrickController.hpp"

enum TowerState {building, active};

class TowerController : Component {
public:
	explicit TowerController(GameObject* gameObject);
	
	void init(bool isStatic);
	void addBrick(BrickController* brick);
private:
	BrickController* structure[16][32][16];
	float fireSpeed;
	float radius;
	int cost;
	std::shared_ptr<ProjectileController> bullet;
};

