#define GLM_ENABLE_EXPERIMENTAL
#include "LevelLoader.hpp"
#include "Grid.hpp"
#include <iostream>
#include <vector>
#include <glm\gtc\matrix_transform.hpp>
#include "ModelLoader.hpp"
#include "GameObject.hpp"
#include "TowerDefense.hpp"


void LevelLoader::generateLevel(std::vector<std::vector<int>> tileValues, glm::vec2& tileSize, std::vector<std::shared_ptr<GameObject>>* gObj) {
	int x, z, tileType;
	for (int i = 0; i < tileValues.size(); i++) {
		std::vector<std::shared_ptr<GameObject>> row;
		for (int j = 0; j < tileValues[0].size(); j++) {
			x = j * tileSize.x;
			z = i * tileSize.y;
			tileType = tileValues[i][j];
			placeTile(tileType, tileSize, x, z, gObj);
		}
	}
}

void LevelLoader::placeTile(int tileType, glm::vec2 tileSize, int x, int z, std::vector<std::shared_ptr<GameObject>>* gObj) {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	TowerDefense::instance->getModelLoader()->loadModel(obj, "sphere", "sphere");
	obj->setPosition(glm::vec3(x, 0, z));
	gObj->push_back(obj);
	/*std::vector<glm::vec3>vertexPositions = {
		glm::vec3(x,0,z), glm::vec3(x,0,z + tileSize.x), glm::vec3(x + tileSize.x,0,z),
		glm::vec3(x + tileSize.x,0,z + tileSize.x), glm::vec3(x + tileSize.x,0,z), glm::vec3(x,0,z + tileSize.x)

	};
	glm::vec3 position(x, 0, z);
	std::shared_ptr<sre::Material> material = sre::Shader::getUnlit()->createMaterial();
	sre::Color color;
	static auto tile = sre::Mesh::create().withPositions(vertexPositions).build();*/
	/*switch (tileType) {
		case 0:
			color = sre::Color(.486, .702, .396, 1);
			break;
		case 1:
			color = sre::Color(.604, .743, .502, 1);
			break;
		default:
			color = sre::Color(0, 0, 1, 1);
	}
	material->setColor(color);
	rp.draw(tile, glm::translate(glm::mat4(1.f), position), material);*/
}