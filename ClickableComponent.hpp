#pragma once
#include "Component.hpp"

// Component which makes a GameObject clickable.
class ClickableComponent : public Component {
public:
	explicit ClickableComponent(GameObject* gameObject);
	std::array<glm::vec3, 2> getBounds();
	void setBounds(std::array<glm::vec3, 2> bounds);
	bool isActive();
	void setActive(bool state);
	glm::vec3 getCenter();
	void cleanComponent() override;

	void click();
private:
	bool selected = false;
	std::array<glm::vec3, 2> bounds;
	bool active = true;
	glm::vec3 center;
};

