#pragma once
#include "Component.hpp"

class BrickController : Component {
public:
	explicit BrickController(GameObject* gameObject, glm::ivec3 position, glm::ivec3 dimensions);

	void init(bool isStatic);
	glm::ivec3 getPosition();
	glm::ivec3 getDimensions();

private:
	glm::ivec3 position, dimensions;
};

