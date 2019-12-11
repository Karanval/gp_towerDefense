#pragma once
#include "Component.hpp"
class FieldController : public Component {
public:
	explicit FieldController(GameObject* gameObject);

	glm::ivec2 getGridPos();
	void setGridPos(glm::ivec2 pos);

private:
	glm::ivec2 gridPosition;
};
