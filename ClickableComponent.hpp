#pragma once
#include "Component.hpp"
class ClickableComponent : public Component {
public:
	explicit ClickableComponent(GameObject* gameObject);

	void click();
private:
	bool selected = false;
};

