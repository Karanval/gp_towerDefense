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
#include "AudioManager.hpp"

enum GameState : std::uint8_t { Running, GameOver };

const static float  PHYSICS_SCALE = 100;

// Base class for initiating and updating game states, objects and drawing.
// Main game class.
class TowerDefense : public b2ContactListener
{
public:
	std::string miscPath = "../data/misc/";
	TowerDefense();
	std::shared_ptr<GameObject> createGameObject();
	std::shared_ptr<ModelLoader> getModelLoader();
	std::shared_ptr<ClickableComponent> screenToClickableObject(glm::vec2 screenCoord);
	std::shared_ptr<ClickableComponent> mouseToClickableObject();
	std::shared_ptr<EnemyController> getClosestEnemy(glm::vec3 pos);
	std::shared_ptr<Grid> getGrid();
	void displayMessage(std::string message, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	void decrementHealthBy(int damage);
	void decrementGoldBy(int gold);
	void incrementHealthBy(int health);
	void incrementGoldBy(int gold);
	void enemyHurt();
	int getGold();
	sre::Camera getCamera();

	static TowerDefense* instance;
	static constexpr float32 timeStep = 1.0f / 60.0f;
	const float  physicsScale = 100;

	bool test = false;


private:
	~TowerDefense();
	void init(); // insantiate game
	void initPhysics(); // instantiate physics
	void update(float deltaTime); // update game
	void updateCamera(float deltaTime); // update game camera
	void updatePhysics(); // update physics of all gameobjects
	void updateFPS(); // update value for FPS-counter
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void handleContact(b2Contact* contact, bool begin);
	void render(); // render next frame
	void keyInput(SDL_Event& event); // manage key inputs
	void mouseInput(SDL_Event& event); // manage mouse inputs
	void mouseClick(SDL_Event& event); // manage mouse-click
	void genLevel(); // generate level used by the game
	void setupCamera(); // setup game camera
	void setupGUI(); // setup all GUI-overlays
	void setupLevel(); // setup the level to be loaded
	void setupLights(); // setup lights in the level, should be done after the level setup
	void setupSpawner(); // setup enemy spawner
	void setupSounds(); // setup game sounds
	void deregisterPhysicsComponent(PhysicsComponent* r); // remove a given physics from the world
	void registerPhysicsComponent(PhysicsComponent* r); // add a given physics to the world
	void drawResourceOverview(); // draw the resource overview (top information)
	void drawBuildingOverview(); // draw building overview (bottom menu when no tower has been selected)
	void drawUpgradeOverview(std::shared_ptr<TowerController> tower); // draw upgrade-menu when a tower has been selected
	void drawGUI(); // draw and manage current frame's GUI
	void drawMessage(); // draw the GUI-message
	void drawStartScreen(); // draw the intro-message
	// Ray-box-intersection test, used for clickableComponent selection
	bool rayBoxTest(std::array<glm::vec3, 2>& ray, std::array<glm::vec3, 2>& box); 
	void restart(); // restart game
	void cleanUpGameObject(int index);

	b2World* world = nullptr;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	std::shared_ptr<SpawnController> spawner;
	std::shared_ptr<AudioManager> audioManager;
	sre::SDLRenderer renderer;
	sre::Camera camera;
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	sre::WorldLights lights;
	glm::vec3 camPos;
	glm::vec3 lookat;
	glm::vec3 upVec;
	std::shared_ptr<Grid> grid = nullptr;
	std::shared_ptr<ModelLoader> modelLoader = nullptr;
	glm::vec2 mousePos;
	std::shared_ptr<ClickableComponent> selectedClickable = nullptr;
	std::shared_ptr<TowerController> towerBeingBuilt = nullptr;
	std::vector<glm::vec2> enemyPath;
	GameState state;

	bool fwd = false;
	bool bwd = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool zoom = false;
	
	float fps = 0.0f;
	float bestFPS = 0.0f;
	float lastBestFPS = 0.0f;
	int frames = 0;
	float lastFPSUpdate = 0.0f;
	float fixedTime = 0.0f;
	int gold = 0;
	int lives = 0;
	bool gameLost = false;
	bool gameWon = false;
	bool endMessageShown = false;
	bool gameStartScreen = true;
	float makeEnemiesTougher = 0;

	// GUI
	ImFont* uiFont;
	ImFont* messageFont;
	int resourceMenuHeight = 50;
	int bottomMenuHeight = 180;
	float slideVal = 1.0f;
	ImVec4 background = ImVec4(0.0f, 0.0f, 0.0f, 0.5);
	ImVec4 borderCol = ImVec4(0.35f, 0.0, 0.5f, 1.0f);
	ImVec4 transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	ImVec2 imgMargin = ImVec2(/*right*/5, /*top*/5);
	std::shared_ptr<sre::Texture> basicImg;
	std::shared_ptr<sre::Texture> backImg;
	std::shared_ptr<sre::Texture> startScreenImg;
	std::shared_ptr<sre::Texture> crossImg;
	std::map<std::string, std::shared_ptr<sre::Texture>> loadedTextures;
	std::string message;
	bool showMessage = false;
	float messageStart;
	float messageFadeTime = 2.0f;
	float messageStayTime = 0.5f;
	ImVec4 messageCol;
	ImVec2 messageWindowSize = ImVec2(512, 80);



	friend class PhysicsComponent;
};


