#include "TowerLoader.hpp"
#include "ModelLoader.hpp"
#include "TowerController.hpp"
#include "BrickController.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>
#include <limits>
#include "TowerDefense.hpp"
#include "ProjectileController.hpp"

using namespace rapidjson;

void TowerLoader::loadTower(std::shared_ptr<GameObject> towerObj, std::vector<std::shared_ptr<GameObject>>* brickObjs,
							std::string towerName) {
	std::ifstream fis(TowerLoader::towerPath + towerName + ".json");
	IStreamWrapper sw(fis);
	Document d;
	d.ParseStream(sw);
	std::shared_ptr<TowerController> towerC = towerObj->addComponent<TowerController>();
	towerObj->name = d["name"].GetString();
	towerC->setCost(d["cost"].GetInt());
	towerC->setDamage(d["damage"].GetInt());
	towerC->setFirerate(d["firerate"].GetFloat());
	towerC->setRadius(d["radius"].GetFloat());
	towerC->setSpeed(d["speed"].GetFloat());
	towerC->setProjectile(d["projectile"].GetString());
	const Value& upgrades = d["upgrades"];
	for (SizeType i = 0; i < upgrades.Size(); i++) towerC->addUpgrade(upgrades[i]["icon"].GetString());
	const Value& bricks = d["bricks"];
	std::array<glm::vec3, 2> boundary = { glm::vec3(FLT_MAX), glm::vec3(FLT_MIN) };
	for (SizeType i = 0; i < bricks.Size(); i++) {
		const Value& brick = bricks[i];
		std::shared_ptr<GameObject> brickObj = GameObject::createGameObject();
		brickObjs->push_back(brickObj);
		std::shared_ptr<BrickController> brickC = brickObj->addComponent<BrickController>();
		std::string objName = brick["objName"].GetString();
		std::string mtlName = brick["mtlName"].GetString();
		const Value& textureFileName = brick["txtFilename"];
		if (textureFileName.IsNull()) TowerDefense::instance->getModelLoader()->loadModel(brickObj, objName, mtlName);
		else TowerDefense::instance->getModelLoader()->loadModel(brickObj, objName, mtlName, textureFileName.GetString());
		brickC->setupDefaultMaterial();
		const Value& brickPos = brick["position"];
		brickC->setLocalPosition(glm::vec3(brickPos["x"].GetFloat(), brickPos["y"].GetFloat(), brickPos["z"].GetFloat()));
		brickObj->setRotation(glm::vec3(0, brick["rotation"].GetFloat(), 0));
		brickC->setTowerController(towerC);
		brickObj->name = objName;
		std::array<glm::vec3, 2> box = brickObj->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax();
		box[0] += brickC->getLocalPosition();
		box[1] += brickC->getLocalPosition();
		for (int i = 0; i < 3; i++) {
			if (box[0][i] < boundary[0][i]) boundary[0][i] = box[0][i]; // min XYZ
			if (box[1][i] > boundary[1][i]) boundary[1][i] = box[1][i]; // max XYZ
		}
	}
	/* add clickable component to the tower, construct boundary based on bricks */
	std::shared_ptr<ClickableComponent> clickable = towerObj->addComponent<ClickableComponent>();
	clickable->setBounds(boundary);
	clickable->setActive(false);

}