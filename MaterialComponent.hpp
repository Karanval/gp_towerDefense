#pragma once
#include "Component.hpp"

// Extension of a component that loads and sets materials for imported objects.
class MaterialComponent : public Component {
public:
	explicit MaterialComponent(GameObject* gameObject);

	void setMaterial(const std::shared_ptr<sre::Material> material);
	void cleanComponent() override;

	std::shared_ptr<sre::Material> getMaterial();
private:
	std::shared_ptr<sre::Material> material;
};

