#include "TowerDefense.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "ModelLoader.hpp"
#include "Grid.hpp"
#include "LevelLoader.hpp"
#include "PhysicsComponent.hpp"
#include "ClickableComponent.hpp"
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
	fixedTime += deltaTime;
	updateCamera(deltaTime);
	updatePhysics();

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->update(deltaTime);
		std::shared_ptr<ProjectileController> pc = gameObjects[i]->getComponent<ProjectileController>();
		//if (pc && pc->isDestinationReached()) gameObjects[i].reset(); // <--------------------------------------------- FIX FIX FIX
	}
}

void TowerDefense::updateCamera(float deltaTime) {
	glm::vec3 fwdVec = glm::vec3(-10.0, 0, 10.0f);
	glm::normalize(fwdVec);
	glm::vec3 leftVec = glm::cross(upVec, fwdVec);
	glm::vec3 zoomDist = zoom ? (lookat - camPos) * 0.80f : glm::vec3();
	
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
		// TODO constant Y
		gameObject->setPosition(glm::vec3((position.x * physicsScale),gameObject->getPosition().y, position.y * physicsScale));
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
		if (go->getComponent<MeshComponent>()) {
			rp.draw(go->getComponent<MeshComponent>()->getMesh(), 
					glm::translate(go->getPosition()) * glm::rotate(glm::radians(go->getRotation()), glm::vec3(0, 1, 0)),
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
		debugDraw.clear();
	}
	drawGUI();
}

void TowerDefense::init() {
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

	// Create Spawner
	std::shared_ptr<GameObject> spawnObj = GameObject::createGameObject();
	spawner = spawnObj->addComponent<SpawnController>();
	spawner->setGameObjects(&gameObjects);
	// TODO: replace with actual path when Grid is ready

	//spawner->startSpawningCycle({glm::vec2(5-3,-4)/*, glm::vec2(-3,-3)*/ });
	spawner->startSpawningCycle(enemyPath);
	gameObjects.push_back(spawnObj);
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

void TowerDefense::setupLights() {
	lights.setAmbientLight(glm::vec3(0.1f, 0.1f, 0.1f));
	glm::vec2 offset = grid->getOffset();
	glm::vec3 size = grid->getTileSize();

	for (int i = 0; i < grid->getHeight(); i++) {
		for (int j = 0; j < grid->getWidth(); j++) {
			sre::Light light = sre::Light();
			light.color = glm::vec3(1.0f, 1.0f, 1.0f);
			light.position = glm::vec3((offset.x + i) * size.x, 2 * size.z, (offset.y + j) * size.y);
			light.lightType = sre::LightType::Point;
			light.range = 100.0f;
			lights.addLight(light);
		}
	}
}

void TowerDefense::setupGUI() {
	// Font
	ImFontAtlas* fonts = ImGui::GetIO().Fonts;
	fonts->AddFontDefault();
	std::string fontName = miscPath + "UIFont.ttf";
	int fontSize = 26;
	uiFont = fonts->AddFontFromFileTTF(fontName.c_str(), fontSize);
	
	// Images
	basicImg = sre::Texture::create().withFile(modelLoader->texturePath + "basic_tower.png")
									 .withFilterSampling(false).build();
	backImg = sre::Texture::create().withFile(modelLoader->texturePath + "back_arrow.png")
		.withFilterSampling(false).build();
}

void TowerDefense::drawResourceOverview() {
	ImVec2 winPos = ImVec2(0, 0);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, resourceMenuHeight);
	ImVec4 background = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 goldTextCol = ImVec4(1.0f, 0.76f, 0.14f, 1.0f);
	ImVec4 lifeTextCol = ImVec4(0.96f, 0.18f, 0.18f, 1.0f);
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
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawBuildingOverview() {
	ImVec2 winPos = ImVec2(0, sre::Renderer::instance->getWindowSize().y - bottomMenuHeight);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, bottomMenuHeight);
	ImVec4 background = ImVec4(0.0f, 0.0f, 0.5f, slideVal);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec2 imgMargin = ImVec2(/*right*/5, /*top*/5);
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
		std::shared_ptr<GameObject> obj = createGameObject();
		TowerLoader::loadTower(obj, &gameObjects, "basic");
	}
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawUpgradeOverview() {
	std::shared_ptr<TowerController> tower = selectedClickable->getGameObject()->getComponent<TowerController>();
	ImVec2 winPos = ImVec2(0, sre::Renderer::instance->getWindowSize().y - bottomMenuHeight);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, bottomMenuHeight);
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(uiFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Button, transparent);
	ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
	ImGui::Begin("upgrades", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	//ImGui::BeginGroup();
	ImGui::Text(tower->getGameObject()->name.c_str());
	ImGui::Text(("Cost: " + std::to_string(tower->getCost())).c_str());
	ImGui::Text(("damage: " + std::to_string(tower->getDamage())).c_str());
	ImGui::Text(("Firerate: " + std::to_string(tower->getFirerate())).c_str());
	ImGui::Text(("radius: " + std::to_string(tower->getRadius())).c_str());
	//ImGui::EndGroup();
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
			std::cout << "UPGRADE: " << tex << "\n";
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
	if (selectedClickable && selectedClickable->getGameObject()->getComponent<TowerController>()) drawUpgradeOverview();
	else drawBuildingOverview();
}

std::shared_ptr<ModelLoader> TowerDefense::getModelLoader() {
	return modelLoader;
}

std::shared_ptr<Grid> TowerDefense::getGrid() {
	return grid;
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


int main() {
	new TowerDefense();

	return 0;
}