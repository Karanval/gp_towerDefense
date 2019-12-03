#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "Box2D/Dynamics/b2World.h"
#include <SDL_events.h>
#include "GameObject.hpp"
#include "SpawnController.hpp"
#include "TowerController.hpp"
#include "BrickController.hpp"
#include "TowerLoader.hpp"
#include "Box2DDebugDraw.hpp"
#include "Grid.hpp"

class TowerDefense : public b2ContactListener
{
public:
	TowerDefense();
	std::string miscPath = "../data/misc/";

	static TowerDefense* instance;
	static constexpr float32 timeStep = 1.0f / 60.0f;

private:
	~TowerDefense();
	void init();
	void initPhysics();
	void update(float deltaTime);
	void updateCamera(float deltaTime);
	void updatePhysics();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void handleContact(b2Contact* contact, bool begin);
	void render();
	void keyInput(SDL_Event& event);
	void mouseInput(SDL_Event& event);
	void drawLevel(sre::RenderPass& rp);
	void setupCamera();
	void setupGUI();
	void setupLevel();
	std::shared_ptr<GameObject> createGameObject();
	void deregisterPhysicsComponent(PhysicsComponent* r);
	void registerPhysicsComponent(PhysicsComponent* r);
	void TowerDefense::drawResourceOverview();
	void TowerDefense::drawBuildingOverview();
	void TowerDefense::drawUpgradeOverview();
	void drawGUI();

	b2World* world = nullptr;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	const float  physicsScale = 100;
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	std::shared_ptr<SpawnController> spawner;
	sre::SDLRenderer renderer;
	sre::Camera camera;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	sre::WorldLights lights;
	glm::vec3 camPos;
	glm::vec3 lookat;
	glm::vec3 upVec;
	ImFont* aceRecordsFont;
	std::unique_ptr<Grid> grid = nullptr;
	
	bool fwd = false;
	bool bwd = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	
	float fixedTime = 0.0f;
	int gold = 0;
	int lives = 0;

	// GUI
	int resourceMenuHeight = 50;
	int bottomMenuHeight = 150;
	int menu = 0; // 0: build, 1: upgrade
	float slideVal = 1.0f;
	bool checkVal = true;
	std::shared_ptr<sre::Texture> gateImg;

	friend class PhysicsComponent;
};


