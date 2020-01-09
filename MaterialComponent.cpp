#include "MaterialComponent.hpp"
#include "GameObject.hpp"

MaterialComponent::MaterialComponent(GameObject* gameObject) : Component(gameObject) {}

void MaterialComponent::setMaterial(const std::shared_ptr<sre::Material> material) {
	this->material = material;
}

std::shared_ptr<sre::Material> MaterialComponent::getMaterial() {
	return material;
}

void MaterialComponent::cleanComponent() {
	material.reset();
	gameObject = nullptr;
}

std::string MaterialComponent::getName() {
	return "Material";
}