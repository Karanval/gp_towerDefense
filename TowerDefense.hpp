#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "Box2D/Dynamics/b2World.h"
#include <SDL_events.h>
#include "GameObject.hpp"
#include "SpawnController.hpp"
#include "TowerController.hpp"
#include "BrickController.hpp"
#include "TowerLoader.hpp"
#include "Box2DDebugDraw.hpp"
#include "Grid.hpp"

class TowerDefense : public b2ContactListener
{
public:
	std::string miscPath = "../data/misc/";
	TowerDefense();
	std::shared_ptr<GameObject> createGameObject();
	std::shared_ptr<ModelLoader> getModelLoader();
	std::shared_ptr<ClickableComponent> TowerDefense::screenToClickableObject(glm::vec2 screenCoord);
	std::shared_ptr<ClickableComponent> TowerDefense::mouseToClickableObject();
	std::shared_ptr<EnemyController> getClosestEnemy(glm::vec3 pos);
	std::shared_ptr<Grid> getGrid();
	void displayMessage(std::string message);
	void TowerDefense::decrementHealthBy(int damage);
	void TowerDefense::decrementGoldBy(int gold);
	void TowerDefense::incrementHealthBy(int health);
	void TowerDefense::incrementGoldBy(int gold);

	static TowerDefense* instance;
	static constexpr float32 timeStep = 1.0f / 60.0f;

private:
	~TowerDefense();
	void init();
	void initPhysics();
	void update(float deltaTime);
	void updateCamera(float deltaTime);
	void updatePhysics();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void handleContact(b2Contact* contact, bool begin);
	void render();
	void keyInput(SDL_Event& event);
	void mouseInput(SDL_Event& event);
	void mouseClick(SDL_Event& event);
	void genLevel();
	void setupCamera();
	void setupGUI();
	void setupLevel();
	void setupLights();
	void deregisterPhysicsComponent(PhysicsComponent* r);
	void registerPhysicsComponent(PhysicsComponent* r);
	void drawResourceOverview();
	void drawBuildingOverview();
	void drawUpgradeOverview();
	void drawGUI();
	void drawMessage();
	bool rayBoxTest(std::array<glm::vec3, 2>& ray, std::array<glm::vec3, 2>& box);

	b2World* world = nullptr;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	const float  physicsScale = 100;
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	std::shared_ptr<SpawnController> spawner;
	sre::SDLRenderer renderer;
	sre::Camera camera;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	sre::WorldLights lights;
	glm::vec3 camPos;
	glm::vec3 lookat;
	glm::vec3 upVec;
	ImFont* uiFont;
	std::shared_ptr<Grid> grid = nullptr;
	std::shared_ptr<ModelLoader> modelLoader = nullptr;
	glm::vec2 mousePos;
	std::shared_ptr<ClickableComponent> selectedClickable = nullptr;
	std::vector<glm::vec2> enemyPath;
	
	bool fwd = false;
	bool bwd = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool zoom = false;
	
	float fixedTime = 0.0f;
	int gold = 0;
	int lives = 0;
	bool gameLost = false;
	bool endMessageShown = false;

	// GUI
	int resourceMenuHeight = 50;
	int bottomMenuHeight = 180;
	float slideVal = 1.0f;
	ImVec4 background = ImVec4(0.0f, 0.0f, 0.5f, slideVal);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec2 imgMargin = ImVec2(/*right*/5, /*top*/5);
	std::shared_ptr<sre::Texture> basicImg;
	std::shared_ptr<sre::Texture> backImg;
	std::map<std::string, std::shared_ptr<sre::Texture>> loadedTextures;
	std::string message;
	bool showMessage = false;
	float messageStart;
	float messageFadeTime = 2.0f;
	float messageStayTime = 3.0f;
	ImVec4 messageCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec2 messageWindowSize = ImVec2(180, 60);


	friend class PhysicsComponent;
};


