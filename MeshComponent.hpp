#pragma once
#include "Component.hpp"

// Extension of a component that loads and sets meshes for imported objects.
class MeshComponent : public Component {
public:
	explicit MeshComponent(GameObject* gameObject);

	void setMesh(const std::shared_ptr<sre::Mesh> mesh);
	std::shared_ptr<sre::Mesh> getMesh();
	void cleanComponent() override;
	std::string getName() override;

private:
	std::shared_ptr<sre::Mesh> mesh = sre::Mesh::create().build();
};

