#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"

enum TowerState {building, active};

class TowerController : public Component {
public:
	explicit TowerController(GameObject* gameObject);
	
	void update(float deltaTime) override;
	void init(bool isStatic);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void onMouse(SDL_Event& event);
	void markDirty();
	bool isDirty();
	void snapToGrid();
	void build();

private:
	glm::vec3 position = glm::vec3();
	float fireSpeed;
	float radius;
	int cost;
	std::shared_ptr<ProjectileController> bullet;
	bool dirty = false;
	bool built = false;
};

