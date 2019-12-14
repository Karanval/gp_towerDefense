[1mdiff --git a/TowerDefense.cpp b/TowerDefense.cpp[m
[1mindex 4979419..942dc3f 100644[m
[1m--- a/TowerDefense.cpp[m
[1m+++ b/TowerDefense.cpp[m
[36m@@ -15,6 +15,15 @@[m [mTowerDefense* TowerDefense::instance = nullptr;[m
 [m
 TowerDefense::TowerDefense() : debugDraw(physicsScale){[m
 	instance = this;[m
[32m+[m	[32msetupGame();[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mTowerDefense::~TowerDefense() {[m
[32m+[m	[32m//ImGui::End();[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid TowerDefense::setupGame() {[m
[32m+[m	[32mprintf("Setting up\n");[m
 	renderer.init();[m
 	init();[m
 [m
[36m@@ -33,8 +42,15 @@[m [mTowerDefense::TowerDefense() : debugDraw(physicsScale){[m
 	renderer.startEventLoop();[m
 }[m
 [m
[31m-TowerDefense::~TowerDefense() {[m
[31m-	//ImGui::End();[m
[32m+[m[32mvoid TowerDefense::cleanUP() {[m
[32m+[m	[32mprintf("Cleaning up\n");[m
[32m+[m	[32mphysicsComponentLookup.clear();[m
[32m+[m
[32m+[m	[32mauto it = gameObjects.begin();[m
[32m+[m
[32m+[m	[32mwhile (it != gameObjects.end()) {[m
[32m+[m		[32mit = gameObjects.erase(it);[m
[32m+[m	[32m}[m
 }[m
 [m
 void TowerDefense::update(float deltaTime) {[m
[36m@@ -53,7 +69,7 @@[m [mvoid TowerDefense::update(float deltaTime) {[m
 [m
 	for (int i = 0; i < toRemove.size(); i++) {[m
 		int index = toRemove[i];[m
[31m-		printf("REMOVED %s\n", gameObjects[index]->name);[m
[32m+[m		[32m//printf("REMOVED %s\n", gameObjects[index]->name);[m
 		auto ec = gameObjects[index]->getComponent<EnemyController>();[m
 		if (ec) gold += ec->getCoinDrop();[m
 [m
[36m@@ -72,6 +88,7 @@[m [mvoid TowerDefense::update(float deltaTime) {[m
 	if (lives <= 0 && !gameLost) {[m
 		//TODO game restart option i.e. press 'space' to restart[m
 		displayMessage("You died!");[m
[32m+[m		[32mstate = GameOver;[m
 		audioManager->play(END_MUSIC);[m
 		gameLost = true;[m
 	}[m
[36m@@ -191,6 +208,7 @@[m [mvoid TowerDefense::render() {[m
 }[m
 [m
 void TowerDefense::init() {[m
[32m+[m	[32mstate = Running;[m
 	if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world[m
 		world->SetContactListener(nullptr);[m
 	}[m
[36m@@ -335,6 +353,13 @@[m [mvoid TowerDefense::keyInput(SDL_Event& event) {[m
 			for (int i = 0; i < gameObjects.size(); i++) if (gameObjects[i]->getComponent<TowerController>())[m
 				gameObjects[i]->getComponent<TowerController>()->setSpeed(gameObjects[i]->getComponent<TowerController>()->getSpeed() - 1);[m
 			break;[m
[32m+[m		[32mcase SDLK_RETURN:[m
[32m+[m			[32mif (state == GameOver) {[m
[32m+[m				[32mprintf("GAME OVER\n");[m
[32m+[m				[32mcleanUP();[m
[32m+[m				[32msetupGame();[m
[32m+[m			[32m}[m
[32m+[m			[32mbreak;[m
 		/* DEBUGGING END */[m
 		}[m
 		break;[m
