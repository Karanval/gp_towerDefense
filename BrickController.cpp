#include "BrickController.hpp"

BrickController::BrickController(GameObject* gameObject, glm::ivec3 position, glm::ivec3 dimensions) : Component(gameObject) {
	BrickController::position = position;
	BrickController::dimensions = dimensions;
}

glm::ivec3 BrickController::getPosition() {
	return BrickController::position;
}
glm::ivec3 BrickController::getDimensions() {
	return BrickController::dimensions;
}
