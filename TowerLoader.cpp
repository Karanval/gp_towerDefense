#include "TowerLoader.hpp"
#include "ModelLoader.hpp"
#include "TowerController.hpp"
#include "BrickController.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace rapidjson;

void TowerLoader::loadTower(std::shared_ptr<GameObject> towerObj, std::vector<std::shared_ptr<GameObject>>* brickObjs,
							std::string towerName) {
	std::ifstream fis(TowerLoader::towerPath + towerName + ".json");
	IStreamWrapper sw(fis);
	Document d;
	d.ParseStream(sw);
	std::shared_ptr<TowerController> towerC = towerObj->addComponent<TowerController>();
	const Value& towerPos = d["position"];
	towerC->setPosition(glm::vec3(towerPos["x"].GetFloat(), towerPos["y"].GetFloat(), towerPos["z"].GetFloat()));
	const Value& bricks = d["bricks"];
	for (SizeType i = 0; i < bricks.Size(); i++) {
		const Value& brick = bricks[i];
		std::shared_ptr<GameObject> brickObj = GameObject::createGameObject();
		brickObjs->push_back(brickObj);
		std::shared_ptr<BrickController> brickC = brickObj->addComponent<BrickController>();
		std::string objName = brick["objName"].GetString();
		std::string mtlName = brick["mtlName"].GetString();
		const Value& textureFileName = brick["txtFilename"];
		if (textureFileName.IsNull()) ModelLoader::loadModel(brickObj, objName, mtlName);
		else ModelLoader::loadModel(brickObj, objName, mtlName, textureFileName.GetString());
		const Value& brickPos = brick["position"];
		brickC->setLocalPosition(glm::vec3(brickPos["x"].GetFloat(), brickPos["y"].GetFloat(), brickPos["z"].GetFloat()));
		brickC->setTowerController(towerC);
	}

}

