#include "TowerController.hpp"
#include "BrickController.hpp"

TowerController::TowerController(GameObject* gameObject) : Component(gameObject) {
}

void TowerController::addBrick(BrickController* brick) {
	glm::ivec3 pos = brick->getPosition();
	glm::ivec3 dim = brick->getDimensions();
	for (int i = pos.x; i < pos.x + dim.x; i++) 
		for (int j = pos.y; j < pos.y + dim.y; j++) 
			for (int k = pos.z; k < pos.z + dim.z; k++) {
				TowerController::structure[i][j][k] = brick;
	}
}



