#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <SDL_events.h>
#include "GameObject.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "ModelLoader.hpp"
#include "ClickableComponent.hpp"

// Function for loading towers according to a JSON file.
namespace TowerLoader
{
	// Path to the tower JSON-files
	const std::string towerPath = "../data/towers/";
	void loadTower(std::shared_ptr<GameObject> towerObj, std::vector<std::shared_ptr<GameObject>>* brickObjs, std::string towerName, bool randomizeColors);
	
};