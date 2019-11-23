#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "sre/SDLRenderer.hpp"
#include "MaterialComponent.hpp"
#include "MeshComponent.hpp"
#include "GameObject.hpp"

MeshComponent::MeshComponent(GameObject* gameObject) : Component(gameObject) {}

void MeshComponent::setMesh(const std::shared_ptr<sre::Mesh> mesh) {
	this->mesh = mesh;
}

std::shared_ptr<sre::Mesh> MeshComponent::getMesh() {
	return mesh;
}
