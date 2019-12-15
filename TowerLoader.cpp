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
#include <time.h>

using namespace rapidjson;

void TowerLoader::loadTower(std::shared_ptr<GameObject> towerObj, std::vector<std::shared_ptr<GameObject>>* brickObjs,
							std::string towerName, bool randomizeColors) {
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
	std::string mtl1, mtl2;
	if (randomizeColors) {
		srand(time(0));
		switch (rand() % 6) {
		case 0: mtl1 = "blue"; break;
		case 1: mtl1 = "red"; break;
		case 2: mtl1 = "green"; break;
		case 3: mtl1 = "grey"; break;
		case 4: mtl1 = "lightgrey"; break;
		case 5: mtl1 = "lightblue"; break;
		default: mtl1 = "blue"; break;
		}
		switch (rand() % 6) {
		case 0: mtl2 = "blue"; break;
		case 1: mtl2 = "red"; break;
		case 2: mtl2 = "green"; break;
		case 3: mtl2 = "grey"; break;
		case 4: mtl2 = "lightgrey"; break;
		case 5: mtl2 = "lightblue"; break;
		default: mtl2 = "blue"; break;
		}
	}
	if ((rand() % 10) < 2) { // You got lucky!
		mtl1 = "gold"; 
		mtl2 = "gold"; 
		towerC->setFirerate(towerC->getFirerate() * 1.5f);  
		towerC->setSpeed(towerC->getSpeed() * 2);
	}
	for (SizeType i = 0; i < bricks.Size(); i++) {
		const Value& brick = bricks[i];
		std::shared_ptr<GameObject> brickObj = GameObject::createGameObject();

		const Value& brickPos = brick["position"];
		brickObjs->push_back(brickObj);
		std::shared_ptr<BrickController> brickC = brickObj->addComponent<BrickController>();
		std::string objName = brick["objName"].GetString();
		std::string mtlName = "";
		if (randomizeColors) {
			if (rand() % 2) mtlName = mtl1;
			else mtlName = mtl2;
		}
		else mtlName = brick["mtlName"].GetString();
		const Value& textureFileName = brick["txtFilename"];
		if (textureFileName.IsNull()) TowerDefense::instance->getModelLoader()->loadModel(brickObj, objName, mtlName);
		else TowerDefense::instance->getModelLoader()->loadModel(brickObj, objName, mtlName, textureFileName.GetString());
		brickC->setupDefaultMaterial();
		brickC->setLocalPosition(glm::vec3(brickPos["x"].GetFloat(), brickPos["y"].GetFloat(), brickPos["z"].GetFloat()));
		brickObj->setRotation(glm::vec3(0, brick["rotation"].GetFloat(), 0));
		if (brick["rotate"].GetBool()) brickC->setRotate(true);
		brickC->setTowerController(towerC);
		brickObj->name = objName;
		std::array<glm::vec3, 2> box = brickObj->getComponent<MeshComponent>()->getMesh()->getBoundsMinMax();
		box[0] += brickC->getLocalPosition();
		box[1] += brickC->getLocalPosition();
		for (int i = 0; i < 3; i++) {
			if (box[0][i] < boundary[0][i]) boundary[0][i] = box[0][i]; // min XYZ
			if (box[1][i] > boundary[1][i]) boundary[1][i] = box[1][i]; // max XYZ
		}
		towerC->addBrick(brickObj);
	}
	/* add clickable component to the tower, construct boundary based on bricks */
	std::shared_ptr<ClickableComponent> clickable = towerObj->addComponent<ClickableComponent>();
	clickable->setBounds(boundary);
	clickable->setActive(false);
}