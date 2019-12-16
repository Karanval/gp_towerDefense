#pragma once
#include "Component.hpp"
#include "GameObject.hpp"
#include "TowerController.hpp"
#include <sre/RenderPass.hpp>

// Class for manipulating the bricks of which the towers are built of.
class BrickController : public Component {
public:
	explicit BrickController(GameObject* gameObject);

	void update(float deltaTime) override;
	void init(bool isStatic);
	void cleanComponent() override;
	glm::vec3 getPosition(); // position in world-space
	glm::vec3 getLocalPosition(); // position in local-space
	void setLocalPosition(glm::vec3 position); // set position relative to the brick's tower
	std::shared_ptr<TowerController> getTowerController(); // get attached tower
	void setTowerController(std::shared_ptr<TowerController> towerController); // set attached tower
	void markDirty(); // tell the brick that it should update its material and position
	bool isDirty(); // returns true when the brick should update
	void setupDefaultMaterial(); // the default material for when the brick is not red (indicating unbuildable tower)
	void setRotate(bool state);  // if true, then the brick rotates around itself, aiming at the tower's target
	

private:
	glm::vec3 position = glm::vec3();
	std::shared_ptr<TowerController> towerController;
	bool dirty = false;
	std::shared_ptr<sre::Material> defaultMaterial;
	std::shared_ptr<sre::Material> unbuildableMaterial;
	bool rotate = false;
};
