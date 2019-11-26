#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include "TowerController.hpp"

class BrickController : public Component {
public:
	explicit BrickController(GameObject* gameObject);

	void init(bool isStatic);
	glm::vec3 getPosition();
	glm::vec3 getLocalPosition();
	void setLocalPosition(glm::vec3 position);
	std::shared_ptr<TowerController> getTowerController();
	void setTowerController(std::shared_ptr<TowerController> towerController);
	

private:
	glm::vec3 position = glm::vec3();
	std::shared_ptr<TowerController> towerController;
};

