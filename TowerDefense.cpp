#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "TowerDefense.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "ModelLoader.hpp"

TowerDefense::TowerDefense() {
	renderer.init();
	init();

	renderer.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};
	renderer.frameRender = [&]() {
		render();
	};
	renderer.keyEvent = [&](SDL_Event& e) {
		keyInput(e);
	};
	renderer.mouseEvent = [&](SDL_Event& e) {
		mouseInput(e);
	};
	renderer.startEventLoop();
}

void TowerDefense::update(float deltaTime) {
	fixedTime += deltaTime;
	glm::vec3 fwdVec = lookat - camPos;
	glm::normalize(fwdVec);
	glm::vec3 leftVec = glm::cross(upVec, fwdVec);
	if (fwd) {
		camPos += fwdVec * 0.05f; 
		lookat += fwdVec * 0.05f;
	}
	else if (bwd) {
		camPos -= fwdVec * 0.05f; 
		lookat -= fwdVec * 0.05f;
	}
	if (left) {
		//camPos -= leftVec * 0.05f; 
		lookat += leftVec * 0.05f;
	}
	else if (right) {
		//camPos += leftVec * 0.05f; 
		lookat -= leftVec * 0.05f;
	}
	if (up) {
		camPos += upVec * 0.05f; 
		lookat += upVec * 0.05f;
	}
	else if (down) {
		camPos -= upVec * 0.05f; 
		lookat -= upVec * 0.05f;
	}
	camera.lookAt(camPos, lookat, upVec);
}

void TowerDefense::render() {
	sre::RenderPass rp = sre::RenderPass::create()
						.withCamera(camera).withWorldLights(&lights)
						.build();

	float x = 0.0f;
	float z = 0.0f;
	
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<GameObject> go = gameObjects[i];
		if (!go->getComponent<MeshComponent>()) continue;
		std::shared_ptr<BrickController> bc = gameObjects[i]->getComponent<BrickController>();
		glm::vec3 pos = bc ? bc->getPosition() : go->getPosition();
		rp.draw(go->getComponent<MeshComponent>()->getMesh(), 
				glm::translate(pos), 
			    go->getComponent<MaterialComponent>()->getMaterial());
		std::vector<glm::vec3> verts = std::vector<glm::vec3>();
	}
}

void TowerDefense::init() {
	lights = sre::WorldLights();

	std::shared_ptr<GameObject> towerObj = createGameObject();
	std::shared_ptr<TowerController> towerController = towerObj->addComponent<TowerController>();
	float x = 0.0f;
	float z = 0.0f;
	for (int i = 0; i < 25; i++) {
		z += 10.0f;
		if (i % 5 == 0) {
			x += 5.0f;
			z = 0.0f;
		}
		std::shared_ptr<GameObject> obj = createGameObject();
		ModelLoader::loadModel(obj, "lego_brick1", "lego_brick1");
		obj->setPosition(glm::vec3(x, 0.0f, z));
		if (i > 10) {
			obj->addComponent<BrickController>()->setTowerController(towerController);
			obj->getComponent<BrickController>()->setLocalPosition(glm::vec3(x, 0.0f, z));
		}

	}

	lights.setAmbientLight(glm::vec3(0.1f,0.1f,0.1f));
	sre::Light light = sre::Light();
	light.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(5.0f, 2.0f, 5.0f);
	light.lightType = sre::LightType::Point;
	light.range = 100.0f;
	lights.addLight(light);

	camPos = glm::vec3(0.0f, 3.0f, -5.0f);
	lookat = glm::vec3(0.0f, -1.0f, 0.0f);
	upVec = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.setPerspectiveProjection(75.0f, 0.1f, 1000.0f);
}

void TowerDefense::keyInput(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			fwd = true;
			break;
		case SDLK_s:
			bwd = true;
			break;
		case SDLK_a:
			left = true;
			break;
		case SDLK_d:
			right = true;
			break;
		case SDLK_SPACE:
			up = true;
			break;
		case SDLK_LCTRL:
			down = true;
			break;
		case SDLK_1:
			for (int i = 0; i < gameObjects.size(); i++) {
				std::shared_ptr<TowerController> tc = gameObjects[i]->getComponent<TowerController>();
				if (tc) tc->setPosition(tc->getPosition() - glm::vec3(1.0f, 0.0f, 0.0f));
			}
			break;
		case SDLK_2:
			for (int i = 0; i < gameObjects.size(); i++) {
				std::shared_ptr<TowerController> tc = gameObjects[i]->getComponent<TowerController>();
				if (tc) tc->setPosition(tc->getPosition() + glm::vec3(1.0f, 0.0f, 0.0f));
			}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			fwd = false;
			break;
		case SDLK_s:
			bwd = false;
			break;
		case SDLK_a:
			left = false;
			break;
		case SDLK_d:
			right = false;
			break;
		case SDLK_SPACE:
			up = false;
			break;
		case SDLK_LCTRL:
			down = false;
			break;
		}
		break;
	}
}

void TowerDefense::mouseInput(SDL_Event& event) {

}

std::shared_ptr<GameObject> TowerDefense::createGameObject() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	gameObjects.push_back(obj);
	return obj;
}

int main() {
	new TowerDefense();

	return 0;
}