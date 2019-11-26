#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"

enum TowerState {building, active};

class TowerController : public Component {
public:
	explicit TowerController(GameObject* gameObject);
	
	void init(bool isStatic);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

private:
	glm::vec3 position = glm::vec3();
	float fireSpeed;
	float radius;
	int cost;
	std::shared_ptr<ProjectileController> bullet;
};

