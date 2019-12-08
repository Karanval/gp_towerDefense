#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"
#include "FieldController.hpp"
#include "MaterialComponent.hpp"

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
	bool isUnbuildable();
	void addUpgrade(std::string upgrade);
	std::vector<std::string>* getUpgrades();
	void setCost(int cost);
	int getCost();
	void setFirerate(float firerate);
	float getFirerate();
	void setRadius(float radius);
	float getRadius();


private:
	glm::vec3 position = glm::vec3();
	std::shared_ptr<sre::Texture> icon;
	int cost;
	float firerate;
	float radius;
	std::shared_ptr<ProjectileController> bullet;
	std::shared_ptr<FieldController> field;
	bool dirty = false;
	bool built = false;
	bool snapping = false;
	bool unbuildable = false;
	std::vector<std::string> upgrades;
};

