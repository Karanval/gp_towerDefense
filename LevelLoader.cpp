#define GLM_ENABLE_EXPERIMENTAL
#include "LevelLoader.hpp"
#include "Grid.hpp"
#include <iostream>
#include <vector>
#include <glm\gtc\matrix_transform.hpp>
#include "ModelLoader.hpp"
#include "GameObject.hpp"
#include "TowerDefense.hpp"
#include "FieldController.hpp"
#include "TowerController.hpp"


void LevelLoader::generateLevel(std::vector<std::vector<int>> tileValues, glm::vec3& tileSize, glm::vec2& offset, 
								std::vector<std::shared_ptr<GameObject>>* gObj) {
	int x, z, tileType;
	for (int i = 0; i < tileValues.size(); i++) {
		std::vector<std::shared_ptr<GameObject>> row;
		for (int j = 0; j < tileValues[0].size(); j++) {
			tileType = tileValues[i][j];
			placeTile(tileType, tileSize, offset, j, i, gObj);
		}
	}
}

void LevelLoader::placeTile(int tileType, glm::vec3 tileSize, glm::vec2 offset, int x, int z, std::vector<std::shared_ptr<GameObject>>* gObj) {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	switch (tileType) {
	case 0: TowerDefense::instance->getModelLoader()->loadModel(obj, "path", "path", "path.png"); break;
	case 1: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain", "terrain", "terrain.png"); break;
	case 2: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_north", "terrain", "terrain.png"); break;
	case 3: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_northeast", "terrain", "terrain.png"); break;
	case 4: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_east", "terrain", "terrain.png"); break;
	case 5: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_southeast", "terrain", "terrain.png"); break;
	case 6: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_south", "terrain", "terrain.png"); break;
	case 7: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_southwest", "terrain", "terrain.png"); break;
	case 8: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_west", "terrain", "terrain.png"); break;
	case 9: TowerDefense::instance->getModelLoader()->loadModel(obj, "terrain_northwest", "terrain", "terrain.png"); break;
	case 10:
		TowerDefense::instance->getModelLoader()->loadModel(obj, "gate", "gate", "gate.png");
		obj->addComponent<TowerController>();
		obj->getComponent<TowerController>()->build();
		break;
	default: TowerDefense::instance->getModelLoader()->loadModel(obj, "path", "path", "path.png"); break; // error
	}
	obj->setPosition(glm::vec3((offset.x + x) * tileSize.x, 0, (offset.y + z) * tileSize.y));
	std::shared_ptr<FieldController> field = obj->addComponent<FieldController>();
	std::shared_ptr<ClickableComponent> clickable = obj->addComponent<ClickableComponent>();
	glm::vec3 half = tileSize * 0.5f;
	float maxY = obj->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax()[1].y;
	std::array<glm::vec3, 2> boundary = { glm::vec3(-half.x, 0, -half.y), glm::vec3(half.x, maxY, half.y) };
	clickable->setBounds(boundary);
	field->setGridPos(glm::ivec2(x, z));
	std::stringstream nameStream;
	nameStream << "Tile " << x << ", " << z;
	obj->name = nameStream.str();
	gObj->push_back(obj);
}
