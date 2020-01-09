#pragma once
#include "Component.hpp"
#include "ProjectileController.hpp"
#include "FieldController.hpp"
#include "MaterialComponent.hpp"

enum TowerState {building, active};

// Class for controlling tower building, destroying, and upgrading.
class TowerController : public Component {
public:
	explicit TowerController(GameObject* gameObject);
	
	void update(float deltaTime) override;
	void init(bool isStatic);
	void onMouse(SDL_Event& event); // pass arguments on key-events
	void markDirty(); // tell the tower that it and its bricks should update their positions
	bool isDirty(); // returns true when the tower must update its position
	void snapToGrid(); // try to snap the tower to the grid's field at the mouse-position
	void build(); // build the tower
	bool isBuilt(); // true when the tower has been built
	bool isUnbuildable(); // true if the tower cannot be built
	void addUpgrade(std::string upgrade); // add a possible upgrade to the tower, used by TowerLoader
	std::vector<std::string>* getUpgrades(); // list of all possible upgrades
	// Cost to build/upgrade into tower
	void setCost(int cost);
	int getCost();
	// Damage of the spawned projectiles
	void setDamage(int damage);
	int getDamage();
	// Firing speed
	void setFirerate(float firerate);
	float getFirerate();
	// Targeting range
	void setRadius(float radius);
	float getRadius();
	// Projectile travelling speed
	void setSpeed(float speed);
	float getSpeed();
	void setProjectile(std::string projectile); // set projectile name, used by TowerLoader
	std::string getProjectile(); // get the projectile name
	void shoot(std::shared_ptr<EnemyController> target); // shoot a projectile at the given target
	void explode(); // make tower explode
	bool isExploding(); // true when tower has exploded
	void destroy(); // remove from existence
	void addBrick(std::shared_ptr<GameObject> brickObj); // add brick to the tower
	std::vector<std::shared_ptr<GameObject>> getBricks(); // access all the tower's bricks
	std::shared_ptr<EnemyController> getTarget(); // get targeted enemy, used by projectiles
	void cleanComponent() override;
	std::string getName() override;


private:
	std::shared_ptr<sre::Texture> icon;
	int cost = 0;
	int damage = 0;
	float firerate = 0.0f;
	float radius = 0.0f;
	float speed = 0.0f;
	float timeSinceBuilt = 0.0f;
	float lastShotTime = 0.0f;
	std::shared_ptr<ProjectileController> bullet;
	std::shared_ptr<FieldController> field;
	std::shared_ptr<EnemyController> target;
	std::unique_ptr<PhysicsComponent> phys = nullptr;
	bool dirty = false;
	bool built = false;
	bool snapping = false;
	bool unbuildable = false;
	bool exploding = false;
	float fallTime = 0;
	std::vector<std::string> upgrades;
	std::string projectile;
	std::vector<std::shared_ptr<GameObject>> bricks;
};

