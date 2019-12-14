#pragma once
#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

// Class for loading 3D objects into SRE.
namespace ObjImporter {
	void loadMaterial(std::shared_ptr<sre::Material> &material, std::string filepath, std::vector<glm::vec4>& uvs,
					  std::string texturePath = "");
	void loadObj(std::shared_ptr<sre::Mesh> &mesh, std::string filepath, std::string &materialFilename);
};

struct face {
	glm::vec3 vertices[3];
	glm::vec2 texCoords[3];
	glm::vec3 normals[3];
};