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
	std::string miscPath = "../data/misc/";

private:
	~TowerDefense();
	void init();
	void update(float deltaTime);
	void updateCamera(float deltaTime);
	void render();
	void keyInput(SDL_Event& event);
	void mouseInput(SDL_Event& event);
	void drawLevel(sre::RenderPass& rp);
	void setupCamera();
	void setupGUI();
	std::shared_ptr<GameObject> createGameObject();
	void drawGUI();
	

	std::shared_ptr<SpawnController> spawner;
	sre::SDLRenderer renderer;
	sre::Camera camera;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	sre::WorldLights lights;
	glm::vec3 camPos;
	glm::vec3 lookat;
	glm::vec3 upVec;
	ImFont* aceRecordsFont;
	
	bool fwd = false;
	bool bwd = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	
	float fixedTime = 0.0f;

	// GUI
	int bottomMenuHeight = 150;
	float slideVal = 1.0f;
	bool checkVal = true;
	std::shared_ptr<sre::Texture> gateImg;

};


