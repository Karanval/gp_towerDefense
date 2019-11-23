#pragma once
#include "Component.hpp"
class MeshComponent : public Component {
public:
	explicit MeshComponent(GameObject* gameObject);

	void setMesh(const std::shared_ptr<sre::Mesh> mesh);
	std::shared_ptr<sre::Mesh> getMesh();
private:
	std::shared_ptr<sre::Mesh> mesh = sre::Mesh::create().build();
};

