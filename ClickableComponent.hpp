#pragma once
#include "Component.hpp"

// Component which makes a GameObject clickable.
class ClickableComponent : public Component {
public:
	explicit ClickableComponent(GameObject* gameObject);
	std::array<glm::vec3, 2> getBounds(); // Axis-Aligned-Bounding-Box
	void setBounds(std::array<glm::vec3, 2> bounds); // set the bounds of the component, this is the clickable area
	bool isActive(); // returns true when the component is active
	void setActive(bool state); // true: clickable, false: ignored
	glm::vec3 getCenter(); // the center of the clickable area
	void cleanComponent() override;
	std::string getName() override;

	void click(); // perform an action. For future work, to assign actions performed when the clickable is clicked
private:
	bool selected = false;
	std::array<glm::vec3, 2> bounds;
	bool active = true;
	glm::vec3 center;
};

