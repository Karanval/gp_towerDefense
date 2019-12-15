#pragma once
#include "Component.hpp"

// Basic component structure for a Field.
// Used for manipulating grid positions.
class FieldController : public Component {
public:
	explicit FieldController(GameObject* gameObject);

	glm::ivec2 getGridPos();
	void setGridPos(glm::ivec2 pos);
	void cleanComponent() override;


private:
	glm::ivec2 gridPosition;
};

