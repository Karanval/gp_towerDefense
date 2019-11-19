#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <SDL_events.h>
class TowerDefense
{
public:
	TowerDefense();
	const std::string objectPath = "./data/models/";
	const std::string materialPath = "./data/materials/";
	const std::string texturePath = "./data/textures/";

private:
	void init();
	void update(float deltaTime);
	void render();
	void keyInput(SDL_Event& event);
	void mouseInput(SDL_Event& event);
	void loadModel(std::string objName, std::string mtlName, std::string textureNameWithExt = "");
	
	sre::SDLRenderer renderer;
	sre::Camera camera;
	std::vector<std::shared_ptr<sre::Mesh>> meshes;
	std::vector<std::shared_ptr<sre::Material>> materials;
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

};

