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
		//std::shared_ptr<BrickController> bc = gameObjects[i]->getComponent<BrickController>();
		//glm::vec3 pos = bc ? bc->getPosition() : go->getPosition();
		rp.draw(go->getComponent<MeshComponent>()->getMesh(), 
				glm::translate(go->getPosition()),
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
	drawGUI();
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
	spawner->startSpawningCycle({glm::vec2(5.0f,0.0f), glm::vec2(7.0f,0.0f) });
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
	setupGUI();
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
		/* DEBUGGING */
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
		case SDLK_3:
			gold++;
			break;
		case SDLK_4:
			gold--;
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

// TODO: make gameObject and component contain functionality for markDirty and bounds, and then override instead
//		 same way as with the update-function
void TowerDefense::mouseClick(SDL_Event& event) {
	float y = sre::Renderer::instance->getWindowSize().y - event.button.y; // invert y-axis
	std::array<glm::vec3, 2> ray = camera.screenPointToRay(glm::vec2(event.button.x, y));
	for (int i = 0; i < gameObjects.size(); i++) {
		std::shared_ptr<MeshComponent> mesh = gameObjects[i]->getComponent<MeshComponent>();
		std::shared_ptr<ClickableComponent> clickable = gameObjects[i]->getComponent<ClickableComponent>();
		/* TODO: make all clickables clickable when getBounds in BrickController is "cohesioned" away */
		std::shared_ptr<BrickController> brickC = gameObjects[i]->getComponent<BrickController>();
		if (mesh && clickable && /*TODO: remove*/ brickC) {
			std::array<glm::vec3, 2> boundary = brickC->getBounds(); /* TODO: update */
			if (rayBoxTest(ray, boundary)) clickable->click();
		}
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
		break;
	}
}

void TowerDefense::drawLevel(sre::RenderPass& rp) {
	std::unique_ptr<Grid> grid = std::make_unique<Grid>();
	std::unique_ptr<LevelLoader> level = std::make_unique<LevelLoader>();
	const std::string mapPath = "../data/maps/";
	grid->loadMap(mapPath + "level0.json");
	level->generateLevel(grid->getTileValues(), grid->getTileSize(), rp);
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

void TowerDefense::setupGUI() {
	// Font
	ImFontAtlas* fonts = ImGui::GetIO().Fonts;
	fonts->AddFontDefault();
	std::string fontName = miscPath + "AceRecords.ttf";
	int fontSize = 26;
	aceRecordsFont = fonts->AddFontFromFileTTF(fontName.c_str(), fontSize);
	
	// Images
	gateImg = sre::Texture::create().withFile(ModelLoader::texturePath + "gate_view.png")
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
	ImGui::PushFont(aceRecordsFont);
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
	bool showBorder = checkVal;
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(aceRecordsFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Button, transparent);
	if (showBorder) ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
	ImGui::Begin("buildings", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("Build");
	ImGui::SetCursorPosX(winSize.x - 128 + imgMargin.x);
	ImGui::SetCursorPosY(winSize.y - bottomMenuHeight + imgMargin.y);
	ImTextureID textureID = gateImg->getNativeTexturePtr();
	if (ImGui::ImageButton(textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0))) menu = 1;
	ImGui::End();
	ImGui::PopFont();
	if (showBorder) ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawUpgradeOverview() {
	ImVec2 winPos = ImVec2(0, sre::Renderer::instance->getWindowSize().y - bottomMenuHeight);
	ImVec2 winSize = ImVec2(sre::Renderer::instance->getWindowSize().x, bottomMenuHeight);
	ImVec4 background = ImVec4(0.0f, 0.0f, 0.5f, slideVal);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec2 imgMargin = ImVec2(/*right*/5, /*top*/5);
	bool showBorder = checkVal;
	ImGui::SetNextWindowPos(winPos, ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(winSize, ImGuiSetCond_Always);
	ImGui::PushFont(aceRecordsFont);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
	ImGui::PushStyleColor(ImGuiCol_Button, transparent);
	if (showBorder) ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
	ImGui::Begin("upgrades", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::BeginGroup();
	ImGui::Text("Upgrade");
	ImGui::SliderFloat("background", &slideVal, 0.0f, 1.0f);
	ImGui::Checkbox("border", &checkVal);
	ImGui::EndGroup();
	ImGui::SetCursorPosX(winSize.x - 128 + imgMargin.x);
	ImGui::SetCursorPosY(winSize.y - bottomMenuHeight + imgMargin.y);
	ImTextureID textureID = gateImg->getNativeTexturePtr();
	if (ImGui::ImageButton(textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0))) menu = 0;
	ImGui::End();
	ImGui::PopFont();
	if (showBorder) ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void TowerDefense::drawGUI() {
	drawResourceOverview();
	switch (menu) {
	case 0: drawBuildingOverview(); break;
	case 1: drawUpgradeOverview(); break;
	}
}

int main() {
	new TowerDefense();

	return 0;
}