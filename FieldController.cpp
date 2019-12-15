#include "FieldController.hpp"
#include "GameObject.hpp"
#include <string>
#include <iostream>
#include "Grid.hpp"

FieldController::FieldController(GameObject* gameObject) : Component(gameObject) {}

glm::ivec2 FieldController::getGridPos() {
	return gridPosition;
}

void FieldController::setGridPos(glm::ivec2 pos) {
	gridPosition = pos;
}

void FieldController::cleanComponent() {

}
