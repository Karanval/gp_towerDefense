#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <SDL_events.h>
#include "GameObject.hpp"
#include <map>

// Class for loading objects, materials and textured for importing models.
class ModelLoader
{
	public:
		ModelLoader();
		// Pre-defined paths used to access external data
		const std::string objectPath = "../data/models/";
		const std::string materialPath = "../data/materials/";
		const std::string texturePath = "../data/textures/";
		// Load object with filename objName, mtlName and textureName. Texturename must contain its extension (e.g. .png)
		// Object is added to given gameobject as a MeshComponent and MaterialComponent.
		void loadModel(std::shared_ptr<GameObject> obj, std::string objName, std::string mtlName, std::string textureNameWithExt = "");

	private:
		std::map<std::string, std::shared_ptr<sre::Mesh>> loadedMeshes;
		std::map<std::string, std::shared_ptr<sre::Material>> loadedMaterials;
};