#include "GameObject.hpp"
#include <cassert>
#include <algorithm>
#include "Component.hpp"
#include <iostream>

GameObject::~GameObject() {
    // remove reference to this in components
    for (auto& c : components){
        c->gameObject = nullptr;
    }
}

bool GameObject::removeComponent(std::shared_ptr<Component> component) {
    auto comp = std::find(components.begin(), components.end(),component);
    if (comp != components.end()){
		delete component.get();
        components.erase(comp);
    }
    return false;
}

void GameObject::cleanUp() {
    //std::cout << "Start cleaning on " << name << " (" << components.size() << " components)\n";
	for (int i = components.size() - 1; i >= 0; i--) {
        //std::cout << "Cleaning " << components[i]->getName() << "\n";
		components[i]->cleanComponent();
	}
    //std::cout << "\n";
	auto it = components.begin();
	while (it != components.end()) {
		it = components.erase(it);
	}
}

const glm::vec3 &GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec3 &position) {
    GameObject::position = position;
}

const glm::vec3 GameObject::getRotation() const {
    return rotation;
}

void GameObject::setRotation(const glm::vec3 &rotation) {
    GameObject::rotation = rotation;
}

void GameObject::update(float deltaTime) {
    for (auto& comp : components){
        comp->update(deltaTime);
    }
}

bool GameObject::isMarkedForDeath() {
	return markedForDeath;
}

void GameObject::die() {
	markedForDeath = true;
}

const std::vector<std::shared_ptr<Component>> &GameObject::getComponents() {
    return components;
}
