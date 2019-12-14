#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <SDL_events.h>
#include "GameObject.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include <map>

// Class for loading objects, materials and textured for importing models.
class ModelLoader
{
	public:
		ModelLoader();
		const std::string objectPath = "../data/models/";
		const std::string materialPath = "../data/materials/";
		const std::string texturePath = "../data/textures/";
		void loadModel(std::shared_ptr<GameObject> obj, std::string objName, std::string mtlName, std::string textureNameWithExt = "");

	private:
		std::map<std::string, std::shared_ptr<MeshComponent>> loadedMeshes;
		std::map<std::string, std::shared_ptr<MaterialComponent>> loadedMaterials;
};