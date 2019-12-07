#include "ClickableComponent.hpp"
#include "GameObject.hpp"
#include <string>
#include <iostream>
#include "MeshComponent.hpp"

ClickableComponent::ClickableComponent(GameObject* gameObject) : Component(gameObject) {}

void ClickableComponent::click() {
	std::cout << gameObject->name << "\n";
}

std::array<glm::vec3, 2> ClickableComponent::getBounds() {
	std::array<glm::vec3, 2> bounds = ClickableComponent::bounds;
	bounds[0] += gameObject->getPosition();
	bounds[1] += gameObject->getPosition();
	return bounds;
}

void ClickableComponent::setBounds(std::array<glm::vec3, 2> bounds) {
	ClickableComponent::bounds = bounds;
}

bool ClickableComponent::isActive() {
	return active;
}

void ClickableComponent::setActive(bool state) {
	active = state;
}

