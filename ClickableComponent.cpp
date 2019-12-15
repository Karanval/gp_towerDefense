#include "ClickableComponent.hpp"
#include "GameObject.hpp"
#include <string>
#include <iostream>
#include "MeshComponent.hpp"

ClickableComponent::ClickableComponent(GameObject* gameObject) : Component(gameObject) {}

void ClickableComponent::click() {
}

std::array<glm::vec3, 2> ClickableComponent::getBounds() {
	std::array<glm::vec3, 2> bounds = ClickableComponent::bounds;
	bounds[0] += gameObject->getPosition();
	bounds[1] += gameObject->getPosition();
	return bounds;
}

void ClickableComponent::setBounds(std::array<glm::vec3, 2> bounds) {
	ClickableComponent::bounds = bounds;
	center = bounds[0] + (bounds[1] - bounds[0]) / 2.0f;
}

bool ClickableComponent::isActive() {
	return active;
}

void ClickableComponent::setActive(bool state) {
	active = state;
}

glm::vec3 ClickableComponent::getCenter() {
	return center + gameObject->getPosition();
}

void ClickableComponent::cleanComponent() {

}
