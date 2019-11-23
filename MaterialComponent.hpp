#pragma once
#include "Component.hpp"
class MaterialComponent : public Component {
public:
	explicit MaterialComponent(GameObject* gameObject);

	void setMaterial(const std::shared_ptr<sre::Material> material);
	std::shared_ptr<sre::Material> getMaterial();
private:
	std::shared_ptr<sre::Material> material;
};

