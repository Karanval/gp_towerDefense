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

class TowerDefense : public b2ContactListener
{
public:
	TowerDefense();

	static TowerDefense* instance;
	static constexpr float32 timeStep = 1.0f / 60.0f;

private:
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
	std::shared_ptr<GameObject> createGameObject();
	void deregisterPhysicsComponent(PhysicsComponent* r);
	void registerPhysicsComponent(PhysicsComponent* r);

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
	
	bool fwd = false;
	bool bwd = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	float fixedTime = 0.0f;

	friend class PhysicsComponent;
};


