#include "TowerDefense.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "ModelLoader.hpp"
#include "Grid.hpp"
#include "AudioManager.hpp"
#include "LevelLoader.hpp"
#include "PhysicsComponent.hpp"
#include "ClickableComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include <limits>

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

TowerDefense::~TowerDefense() {
	//ImGui::End();
}

void TowerDefense::update(float deltaTime) {
	if (endMessageShown) return;
	fixedTime += deltaTime;
	updateCamera(deltaTime);
	updatePhysics();
	updateFPS();

	std::vector<int> toRemove;
	for (int i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->isMarkedForDeath()) {
			toRemove.push_back(i);
		}
		else gameObjects[i]->update(deltaTime);
	}
	
	for (int i = toRemove.size() - 1; i >= 0; i--) {
		int index = toRemove[i];
		std::cout << gameObjects[index]->name << "\n";
		
		cleanUpGameObject(index);
	}

	if (lives <= 0 && !gameLost) {
		displayMessage("You died! Press 'Enter' to restart.");
		state = GameOver;
		audioManager->play(END_MUSIC);
		gameLost = true;
	}

	if (lives > 0 && gold >= 100) {
		displayMessage("You won! Press 'Enter' to restart.");
		state = GameOver;
		audioManager->play(END_MUSIC);
		gameWon = true;
	}
}

void TowerDefense::updateCamera(float deltaTime) {
	glm::vec3 fwdVec = glm::vec3(-10.0, 0, 10.0f);
	glm::normalize(fwdVec);
	glm::vec3 leftVec = glm::cross(upVec, fwdVec);
	glm::vec3 zoomDist = zoom ? (lookat - camPos) * 0.50f : glm::vec3();
	
	if (fwd) {
		camPos += fwdVec * 0.05f;
		lookat += fwdVec * 0.05f;
	}
	else if (bwd) {
		camPos -= fwdVec * 0.05f;
		lookat -= fwdVec * 0.05f;
	}
	if (left) {
		camPos -= leftVec * 0.05f; 
		lookat += leftVec * 0.05f;
	}
	else if (right) {
		camPos += leftVec * 0.05f; 
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
	camera.lookAt(camPos + zoomDist, lookat, upVec);
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

		if (!gameObject) continue;
		gameObject->setPosition(glm::vec3((position.x * physicsScale),gameObject->getPosition().y, position.y * physicsScale));
		gameObject->setRotation(gameObject->getRotation() + glm::vec3(0, 0, angle));
	}
}

void TowerDefense::updateFPS() {
	float fpsTime = fixedTime - lastFPSUpdate; // time since last FPS update
	fps = glm::mix(lastBestFPS, frames++ / (fixedTime - lastFPSUpdate), fpsTime); // lerp between prev. best and current FPS
	if (fpsTime > 1.0f) { // recalculate FPS to better detect sudden lagg-spikes
		lastFPSUpdate = fixedTime;
		frames = 0;
		lastBestFPS = bestFPS;
	}
	else if (fps > bestFPS) bestFPS = fps;
}

void TowerDefense::render() {
	sre::RenderPass rp = sre::RenderPass::create()
						.withCamera(camera).withWorldLights(&lights)
						.withClearColor(true, sre::Color(0.0f, 1.0f, 1.0f, 1.0f))
						.build();

	float x = 0.0f;
	float z = 0.0f;
	
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<GameObject> go = gameObjects[i];
		if (go->getComponent<MeshComponent>()) {
			rp.draw(go->getComponent<MeshComponent>()->getMesh(), 
					glm::translate(go->getPosition()) * 
					glm::rotate(glm::radians(go->getRotation().x), glm::vec3(1, 0, 0)) *
					glm::rotate(glm::radians(go->getRotation().y), glm::vec3(0, 1, 0)) *
					glm::rotate(glm::radians(go->getRotation().z), glm::vec3(0, 0, 1)),
					go->getComponent<MaterialComponent>()->getMaterial());
		}
		if (doDebugDraw) {
			std::shared_ptr<ClickableComponent> cc = go->getComponent<ClickableComponent>();
			if (cc) {
				std::array<glm::vec3, 2> b = cc->getBounds();
				rp.drawLines({glm::vec3(b[0].x, b[0].y, b[0].z), glm::vec3(b[1].x, b[0].y, b[0].z), // bottom
							  glm::vec3(b[1].x, b[0].y, b[0].z), glm::vec3(b[1].x, b[0].y, b[1].z),
							  glm::vec3(b[1].x, b[0].y, b[1].z), glm::vec3(b[0].x, b[0].y, b[1].z),
							  glm::vec3(b[0].x, b[0].y, b[1].z), glm::vec3(b[0].x, b[0].y, b[0].z),
							  glm::vec3(b[0].x, b[1].y, b[0].z), glm::vec3(b[1].x, b[1].y, b[0].z), // top
							  glm::vec3(b[1].x, b[1].y, b[0].z), glm::vec3(b[1].x, b[1].y, b[1].z),
							  glm::vec3(b[1].x, b[1].y, b[1].z), glm::vec3(b[0].x, b[1].y, b[1].z),
							  glm::vec3(b[0].x, b[1].y, b[1].z), glm::vec3(b[0].x, b[1].y, b[0].z),
							  glm::vec3(b[0].x, b[0].y, b[0].z), glm::vec3(b[0].x, b[1].y, b[0].z), // sides
							  glm::vec3(b[1].x, b[0].y, b[0].z), glm::vec3(b[1].x, b[1].y, b[0].z),
							  glm::vec3(b[1].x, b[0].y, b[1].z), glm::vec3(b[1].x, b[1].y, b[1].z),
							  glm::vec3(b[0].x, b[0].y, b[1].z), glm::vec3(b[0].x, b[1].y, b[1].z)},
					         cc == selectedClickable ? sre::Color(1, 1, 0) : (cc->isActive() ? sre::Color(0, 1, 0) : sre::Color(1, 0, 0)));
			}
		}
	}

	if (doDebugDraw) {
		world->DrawDebugData();
		rp.drawLines(debugDraw.getLines());
		for (int l = 0; l < lights.lightCount(); l++) {
			rp.draw(sre::Mesh::create().withCube(1.0f).build(), glm::translate(lights.getLight(l)->position), sre::Shader::getUnlit()->createMaterial());
		}
		debugDraw.clear();
	}
	drawGUI();
}

void TowerDefense::init() {
	state = Running;
	if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
		world->SetContactListener(nullptr);
	}
	initPhysics();
	lights = sre::WorldLights();

	modelLoader = std::make_shared<ModelLoader>();

	
	setupCamera();
	setupGUI();
	setupLevel();
	setupLights();
	setupSpawner();
	setupSounds();
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

void TowerDefense::handleContact(b2Contact* contact, bool begin) {
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();
	auto physA = physicsComponentLookup.find(fixA);
	auto physB = physicsComponentLookup.find(fixB);
	if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
		auto& aComponents = physA->second->getGameObject()->getComponents();
		auto& bComponents = physB->second->getGameObject()->getComponents();
		for (auto& c : aComponents) {
			if (begin) {
				c->onCollisionStart(physB->second);
			}
			else {
				c->onCollisionEnd(physB->second);
			}
		}
		for (auto& c : bComponents) {
			if (begin) {
				c->onCollisionStart(physA->second);
			}
			else {
				c->onCollisionEnd(physA->second);
			}
		}
	}
}

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
			break;
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
		case SDLK_z:
			zoom = !zoom;
			break;
		case SDLK_m:
			audioManager->pause();
			break;
		/* DEBUGGING */
		case SDLK_1:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setRadius(gameObjects[i]->getComponent<TowerController>()->getRadius() + 1);
			break;
		case SDLK_2:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setRadius(gameObjects[i]->getComponent<TowerController>()->getRadius() - 1);
			break;
		case SDLK_3:
			gold++;
			break;
		case SDLK_4:
			gold--;
			break;
		case SDLK_5:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setFirerate(gameObjects[i]->getComponent<TowerController>()->getFirerate() + 1);
			break;
		case SDLK_6:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setFirerate(gameObjects[i]->getComponent<TowerController>()->getFirerate() - 1);
			break;
		case SDLK_7:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setSpeed(gameObjects[i]->getComponent<TowerController>()->getSpeed() + 1);
			break;
		case SDLK_8:
			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())
				gameObjects[i]->getComponent<TowerController>()->setSpeed(gameObjects[i]->getComponent<TowerController>()->getSpeed() - 1);
			break;
		case SDLK_RETURN:
			//if (state == GameOver) {
				restart();
				state = Running;
				endMessageShown = false;
				gameLost = false;
				showMessage = false;
			//}
			break;
		/* DEBUGGING END */
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

// http://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html		
bool TowerDefense::rayBoxTest(std::array<glm::vec3, 2>& ray, std::array<glm::vec3, 2>& box) {
	float tmin = FLT_MIN, tmax = FLT_MAX;
	for (int i = 0; i < 3; i++) {
		float invDir = 1.0f / ray[1][i];
		float t0 = (box[0][i] - ray[0][i]) * invDir;
		float t1 = (box[1][i] - ray[0][i]) * invDir;
		if (invDir < 0.0f) std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin) return false;
	}
	return true;
}

std::shared_ptr<ClickableComponent> TowerDefense::screenToClickableObject(glm::vec2 screenCoord) {
	std::array<glm::vec3, 2> ray = camera.screenPointToRay(screenCoord);
	std::shared_ptr<ClickableComponent> closestClickable = nullptr;
	float closestDist = FLT_MAX;
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<ClickableComponent> clickable = gameObjects[i]->getComponent<ClickableComponent>();
		if (clickable && clickable->isActive() && rayBoxTest(ray, clickable->getBounds())) {
			glm::length_t distToClickable = glm::distance(camera.getPosition(), clickable->getCenter());
			if (distToClickable < closestDist) {
				closestDist = distToClickable;
				closestClickable = clickable;
			}
		}
	}
	return closestClickable;
}

std::shared_ptr<ClickableComponent> TowerDefense::mouseToClickableObject() {
	float y = sre::Renderer::instance->getWindowSize().y - mousePos.y; // invert y-axis
	return screenToClickableObject(glm::vec2(mousePos.x, y));
}

void TowerDefense::mouseClick(SDL_Event& event) {
	float y = sre::Renderer::instance->getWindowSize().y - event.button.y; // invert y-axis
	if (y > bottomMenuHeight) {
		std::shared_ptr<ClickableComponent> clickable = screenToClickableObject(glm::vec2(event.button.x, y));
		if (clickable) {
			clickable->click();
			selectedClickable = clickable;
		}
		else selectedClickable = nullptr;
	}
}

void TowerDefense::mouseInput(SDL_Event& event) {
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<TowerController> towerC = gameObjects[i]->getComponent<TowerController>();
		if (towerC) towerC->onMouse(event);
	}
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		mouseClick(event);
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEMOTION:
		mousePos.x = event.motion.x;
		mousePos.y = event.motion.y;
		break;
	}
}

void TowerDefense::setupLevel() {
	grid = std::make_unique<Grid>();
	const std::string mapPath = "../data/maps/";
	grid->loadMap(mapPath + "level0.json");
	genLevel();
	enemyPath = grid->getEnemyPath();
}

void TowerDefense::genLevel() {
	std::unique_ptr<LevelLoader> level = std::make_unique<LevelLoader>();
	auto tileValues = grid->getTileValues();
	auto tileSize = grid->getTileSize();
	auto offset = grid->getOffset();
	level->generateLevel(grid->getTileValues(), tileSize, offset, &gameObjects);
}

std::shared_ptr<GameObject> TowerDefense::createGameObject() {
	std::shared_ptr<GameObject> obj = GameObject::createGameObject();
	gameObjects.push_back(obj);
	return obj;
}

void TowerDefense::setupCamera() {
	camPos = glm::vec3(300.0f, 300.0f, -300.0f);
	lookat = glm::vec3(0.0f, 0.0f, 0.0f);
	upVec = glm::vec3(0.0f, 1.0f, 0.0f);

	camera.setPerspectiveProjection(35.264f, 0.1f, 700.0f);
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

void TowerDefense::setupLights() {
	lights.setAmbientLight(glm::vec3(0.1f, 0.1f, 0.1f));
	glm::vec2 offset = grid->getOffset();
	glm::vec3 size = grid->getTileSize();

	for (int i = 0; i < grid->getHeight() / size.y; i++) {
		for (int j = 0; j < grid->getWidth() / size.x; j++) {
			sre::Light light = sre::Light();
			light.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light.position = glm::vec3((offset.x + i) * size.x, 2 * size.y, (offset.y + j) * size.z);
			light.lightType = sre::LightType::Point;
			light.range = 1000.0f;
			lights.addLight(light);
		}
	}
}


void TowerDefense::setupSpawner() {
	// Create Spawner
	std::shared_ptr<GameObject> spawnObj = GameObject::createGameObject();
	spawner = spawnObj->addComponent<SpawnController>();
	spawner->setGameObjects(&gameObjects);

	//spawner->startSpawningCycle({glm::vec2(5-3,-4)/*, glm::vec2(-3,-3)*/ });
	spawner->startSpawningCycle(enemyPath);
	gameObjects.push_back(spawnObj);
}

void TowerDefense::setupSounds() {
	std::shared_ptr<GameObject> am = GameObject::createGameObject();
	audioManager = am->addComponent<AudioManager>();
	// Static variable comes from AudioManager
	audioManager->play(MAIN_MUSIC);
}

void TowerDefense::setupGUI() {
	// Font
	ImFontAtlas* fonts = ImGui::GetIO().Fonts;
	fonts->AddFontDefault();
	std::string fontName = miscPath + "UIFont.ttf";
	uiFont = fonts->AddFontFromFileTTF(fontName.c_str(), 26);
	messageFont = fonts->AddFontFromFileTTF(fontName.c_str(), 36);
	
	// Images
	basicImg = sre::Texture::create().withFile(modelLoader->texturePath + "basic_tower.png")
									 .withFilterSampling(false).build();
	backImg = sre::Texture::create().withFile(modelLoader->texturePath + "back_arrow.png")
		.withFilterSampling(false).build();
}

void TowerDefense::drawResourceOverview() {
	ImVec2 winPos = ImVec2(0, 0);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, resourceMenuHeight);
	ImVec4 background = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//ImVec4 background = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 goldTextCol = ImVec4(1.0f, 0.76f, 0.14f, 1.0f);
	ImVec4 lifeTextCol = ImVec4(0.96f, 0.18f, 0.18f, 1.0f);
	ImVec4 textCol = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(uiFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Border, background);
	ImGui::Begin("resources", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::PushStyleColor(ImGuiCol_Text, goldTextCol);
	ImGui::Text("gold %i", gold);
	ImGui::PopStyleColor();
	ImGui::SameLine(0.0f, 40.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, lifeTextCol);
	ImGui::Text("lives %i", lives);
	ImGui::PopStyleColor();
	ImGui::PushStyleColor(ImGuiCol_Text, textCol);
	ImGui::SameLine(winSize.x - 120);
	ImGui::Text("FPS: %f", fps);
	ImGui::SameLine(winSize.x / 2);
	ImGui::Text("Time: %i seconds", (int)fixedTime);
	ImGui::PopStyleColor();
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawBuildingOverview() {
	ImVec2 winPos = ImVec2(0, sre::Renderer::instance->getWindowSize().y - bottomMenuHeight);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, bottomMenuHeight);
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(uiFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Button, transparent);
	ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
	ImGui::Begin("buildings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("Build");
	ImGui::SetCursorPosX(winSize.x - 128 + imgMargin.x);
	ImGui::SetCursorPosY(winSize.y - bottomMenuHeight + imgMargin.y);
	if (ImGui::ImageButton(basicImg->getNativeTexturePtr(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0))) {
		if (towerBeingBuilt) {
			if (!towerBeingBuilt->isBuilt()) {
				towerBeingBuilt->destroy();
			}
			towerBeingBuilt.reset();
		}
		std::shared_ptr<GameObject> obj = createGameObject();
		TowerLoader::loadTower(obj, &gameObjects, "basic", true);
		towerBeingBuilt = obj->getComponent<TowerController>();

	}
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawUpgradeOverview(std::shared_ptr<TowerController> tower) {
	ImVec2 winPos = ImVec2(0, sre::Renderer::instance->getWindowSize().y - bottomMenuHeight);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, bottomMenuHeight);
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(uiFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Button, transparent);
	ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
	ImGui::Begin("upgrades", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text(tower->getGameObject()->name.c_str());
	ImGui::Text(("Cost: " + std::to_string(tower->getCost())).c_str());
	ImGui::Text(("damage: " + std::to_string(tower->getDamage())).c_str());
	ImGui::Text(("Firerate: " + std::to_string(tower->getFirerate())).c_str());
	ImGui::Text(("radius: " + std::to_string(tower->getRadius())).c_str());
	std::vector<std::string> *upgrades = tower->getUpgrades();
	for (int i = 0; i < upgrades->size(); i++) {
		ImGui::SetCursorPosY(winSize.y - bottomMenuHeight + imgMargin.y);
		ImGui::SetCursorPosX(winSize.x - 56 - (56 + imgMargin.x) * (i + 1));
		std::string tex = upgrades->at(i);
		if (loadedTextures.find(tex) == loadedTextures.end()) {
			loadedTextures.insert(std::pair<std::string, std::shared_ptr<sre::Texture>>
							(tex, sre::Texture::create().withFile(modelLoader->texturePath + tex).withFilterSampling(false).build()));
		}
		if (ImGui::ImageButton(loadedTextures.at(tex)->getNativeTexturePtr(), ImVec2(56, 56), ImVec2(0, 1), ImVec2(1, 0))) {
			if (tex == "cross.png") tower->explode();
			else {
				std::string name = tex.erase(tex.find('.'));
				std::shared_ptr<GameObject> newTowerObj = createGameObject();
				TowerLoader::loadTower(newTowerObj, &gameObjects, name, true);
				std::shared_ptr<TowerController> newTower = newTowerObj->getComponent<TowerController>();
				if (gold < newTower->getCost()) {
					displayMessage("Not enough gold!", ImVec4(1.0f, 0.8f, 0.05f, 1.0f));
					newTower->destroy();
				}
				else {
					glm::vec3 pos = tower->getGameObject()->getPosition();
					tower->destroy();
					newTowerObj->setPosition(pos);
					newTowerObj->getComponent<TowerController>()->build();
					newTower->markDirty();
					selectedClickable = newTowerObj->getComponent<ClickableComponent>();
				}
			}
		}
	}
	ImGui::SetCursorPosY(winSize.y - bottomMenuHeight + imgMargin.y);
	ImGui::SetCursorPosX(winSize.x - 56 - imgMargin.x);
	if (ImGui::ImageButton(backImg->getNativeTexturePtr(), ImVec2(56, 56), ImVec2(0, 1), ImVec2(1, 0))) selectedClickable = nullptr;
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawGUI() {
	drawResourceOverview();
	std::shared_ptr<TowerController> tower = nullptr;
	if (selectedClickable && selectedClickable->getGameObject()) {
		tower = selectedClickable->getGameObject()->getComponent<TowerController>();
		if (tower && tower->isExploding()) selectedClickable = nullptr;
	}
	if (selectedClickable && tower) drawUpgradeOverview(tower);
	else drawBuildingOverview();
	if (showMessage) drawMessage();
}

std::shared_ptr<ModelLoader> TowerDefense::getModelLoader() {
	return modelLoader;
}

std::shared_ptr<Grid> TowerDefense::getGrid() {
	return grid;
}

sre::Camera TowerDefense::getCamera() {
	return camera;
}

std::shared_ptr<EnemyController> TowerDefense::getClosestEnemy(glm::vec3 pos) {
	std::shared_ptr<EnemyController> closestEnemy = nullptr;
	float closestDist = FLT_MAX;
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<EnemyController> enemy = gameObjects[i]->getComponent<EnemyController>();
		float dist = glm::distance(pos, gameObjects[i]->getPosition());
		if (enemy && dist < closestDist) {
			closestEnemy = enemy;
			closestDist = dist;
		}
	}

	return closestEnemy;
}

void TowerDefense::decrementHealthBy(int damage) {
	lives -= damage;
}
void TowerDefense::decrementGoldBy(int gold) {
	TowerDefense::gold -= gold;
}
void TowerDefense::incrementHealthBy(int health) {
	TowerDefense::lives += health;
}
void TowerDefense::incrementGoldBy(int gold) {
	TowerDefense::gold += gold;
}

void TowerDefense::displayMessage(std::string message, ImVec4 color) {
	if (gameLost || gameWon) return;
	TowerDefense::message = message;
	messageCol = color;
	showMessage = true;
	messageStart = fixedTime;
}

void TowerDefense::drawMessage() {
	float t = fixedTime - messageStart;
	float a = 1.0f;
	if (t < messageFadeTime) { // fade in
		a = t / messageFadeTime;
	}
	else if (t < messageFadeTime + messageStayTime) { // stay
		if (gameLost) endMessageShown = true;
	}
	else if (t < messageFadeTime + messageStayTime + messageFadeTime) { // fade out
		a = 1.0f - (t - messageFadeTime - messageStayTime) / messageFadeTime;
	}
	else { // message end
		showMessage = false;
		return;
	}
	messageCol.w = a;
	ImGui::SetNextWindowPos(ImVec2((sre::Renderer::instance->getWindowSize().x - messageWindowSize.x) / 2,
								   sre::Renderer::instance->getWindowSize().y / 2 - messageWindowSize.y), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(messageWindowSize, ImGuiSetCond_Always);
	ImGui::PushFont(messageFont);
	ImGui::PushStyleColor(ImGuiCol_Text, messageCol);
	ImGui::PushStyleColor(ImGuiCol_Border, transparent);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, transparent);
	ImGui::Begin("MessageText", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text(message.c_str());
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

int TowerDefense::getGold() {
	return gold;
}

void TowerDefense::restart() {
	while (!gameObjects.empty()) {
		//This removes the object
		cleanUpGameObject(0);
	}

	//setup level
	fps = 0.0f;
	bestFPS = 0.0f;
	lastBestFPS = 0.0f;
	frames = 0;
	lastFPSUpdate = 0.0f;
	fixedTime = 0.0f;
	gold = 0;
	lives = 0;
	gameLost = false;
	gameWon = false;
	endMessageShown = false;
	setupLevel();
	setupSpawner();
	setupSounds();
}

void TowerDefense::cleanUpGameObject(int index) {
	std::shared_ptr<PhysicsComponent> phys = gameObjects[index]->getComponent<PhysicsComponent>();
	if (phys) {
		auto physB = physicsComponentLookup.find(phys->fixture);
		deregisterPhysicsComponent(physB->second);
	}

	gameObjects[index]->cleanUp();

	auto iterator = gameObjects.begin() + index;
	gameObjects.erase(iterator);
}

int main() {
	new TowerDefense();

	return 0;
}