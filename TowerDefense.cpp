#include "TowerDefense.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "ModelLoader.hpp"
#include "Grid.hpp"
#include "LevelLoader.hpp"
#include "PhysicsComponent.hpp"

TowerDefense* TowerDefense::instance = nullptr;

TowerDefense::TowerDefense() : debugDraw(physicsScale){
	instance = this;
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
	updateCamera(deltaTime);
	updatePhysics();
	
}

void TowerDefense::updateCamera(float deltaTime) {
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

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->update(deltaTime);
	}
}

void TowerDefense::updatePhysics() {
	const float32 timeStep = 1.0f / 60.0f;
	const int positionIterations = 2;
	const int velocityIterations = 6;
	world->Step(timeStep, velocityIterations, positionIterations);

	for (auto phys : physicsComponentLookup) {
		if (phys.second->rbType == b2_staticBody) continue; 
		auto position = phys.second->body->GetPosition();
		float angle = phys.second->body->GetAngle();
		auto gameObject = phys.second->getGameObject();
		//TODO add actual z
		gameObject->setPosition(glm::vec3((position.x * physicsScale), position.y * physicsScale, 5));
		gameObject->setRotation(angle);
	}
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
	//TODO uncomment
	drawLevel(rp);

	if (doDebugDraw) {
		world->DrawDebugData();
		rp.drawLines(debugDraw.getLines());
		debugDraw.clear();
	}
}

void TowerDefense::init() {
	if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
		world->SetContactListener(nullptr);
	}
	initPhysics();
	lights = sre::WorldLights();

	// Create Spawner
	std::shared_ptr<GameObject> spawnObj = GameObject::createGameObject();
	spawner = spawnObj->addComponent<SpawnController>();
	spawner->setGameObjects(&gameObjects);
	// TODO: replace with actual path when Grid is ready
	spawner->startSpawningCycle({glm::vec2(5.0f,0.0f), glm::vec2(10.0f,0.0f) });
	gameObjects.push_back(spawnObj);

	std::shared_ptr<GameObject> obj = createGameObject();
	TowerLoader::loadTower(obj, &gameObjects, "sample");
	

	/*std::shared_ptr<GameObject> towerObj = createGameObject();
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

	}*/

	lights.setAmbientLight(glm::vec3(0.1f,0.1f,0.1f));
	sre::Light light = sre::Light();
	light.color = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(5.0f, 2.0f, 5.0f);
	light.lightType = sre::LightType::Point;
	light.range = 100.0f;
	lights.addLight(light);

	setupCamera();
}

void TowerDefense::initPhysics() {
	float gravity = 0; 
	delete world;
	world = new b2World(b2Vec2(0, gravity));
	world->SetContactListener(this);

	if (doDebugDraw) {
		world->SetDebugDraw(&debugDraw);
	}
}

void TowerDefense::BeginContact(b2Contact* contact) {
	b2ContactListener::BeginContact(contact);
	handleContact(contact, true);
}

void TowerDefense::EndContact(b2Contact* contact) {
	b2ContactListener::EndContact(contact);
	handleContact(contact, false);
}

void TowerDefense::handleContact(b2Contact* contact, bool begin) {}

void TowerDefense::keyInput(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_k:
			// press 'k' for physics debug
			doDebugDraw = !doDebugDraw;
			if (doDebugDraw)
				world->SetDebugDraw(&debugDraw);
			else
				world->SetDebugDraw(nullptr);
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

void TowerDefense::drawLevel(sre::RenderPass& rp) {
	std::unique_ptr<Grid> grid = std::make_unique<Grid>();
	std::unique_ptr<LevelLoader> level = std::make_unique<LevelLoader>();
	const std::string mapPath = "../data/maps/";
	grid->loadMap(mapPath + "level0.json");
	level->generateLevel(grid->getTileValues(), rp);
}

std::shared_ptr<GameObject> TowerDefense::createGameObject() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	gameObjects.push_back(obj);
	return obj;
}

void TowerDefense::setupCamera() {
	camPos = glm::vec3(-300.0f, 300.0f, -300.0f);
	lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	upVec = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.setPerspectiveProjection(800.0f, 0.1f, 1000.0f);
}

void TowerDefense::deregisterPhysicsComponent(PhysicsComponent* r) {
	auto iter = physicsComponentLookup.find(r->fixture);
	if (iter != physicsComponentLookup.end()) {
		physicsComponentLookup.erase(iter);
	}
}

void TowerDefense::registerPhysicsComponent(PhysicsComponent* r) {
	physicsComponentLookup[r->fixture] = r;
}

int main() {
	new TowerDefense();

	return 0;
}