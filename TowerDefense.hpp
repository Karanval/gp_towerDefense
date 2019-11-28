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
class TowerDefense
{
public:
	TowerDefense();

private:
	void init();
	void update(float deltaTime);
	void updateCamera(float deltaTime);
	void render();
	void keyInput(SDL_Event& event);
	void mouseInput(SDL_Event& event);
	void drawLevel(sre::RenderPass& rp);
	void setupCamera();
	std::shared_ptr<GameObject> createGameObject();

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

};


