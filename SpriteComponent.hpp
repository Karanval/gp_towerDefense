#pragma once
#include "Component.hpp"
class SpriteComponent : public Component {
public:
	explicit SpriteComponent(GameObject* gameObject);

	void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) override;

	void setSprite(const sre::Sprite& sprite);
	sre::Sprite getSprite();
	void cleanComponent() override;

private:
	sre::Sprite sprite;
};

