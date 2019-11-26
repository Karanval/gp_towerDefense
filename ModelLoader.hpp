#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <SDL_events.h>
#include "GameObject.hpp"
namespace ModelLoader
{
	const std::string objectPath = "../data/models/";
	const std::string materialPath = "../data/materials/";
	const std::string texturePath = "../data/textures/";
	void loadModel(std::shared_ptr<GameObject> obj, std::string objName, std::string mtlName, std::string textureNameWithExt = "");
};