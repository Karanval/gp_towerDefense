#include "ClickableComponent.hpp"
#include "GameObject.hpp"
#include <string>
#include <iostream>

ClickableComponent::ClickableComponent(GameObject* gameObject) : Component(gameObject) {}

void ClickableComponent::click() {
	std::cout << gameObject->name << "\n";
}
