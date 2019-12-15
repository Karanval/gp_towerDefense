#include "SpriteComponent.hpp"
#include "GameObject.hpp"

SpriteComponent::SpriteComponent(GameObject* gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {
	sprite.setPosition(gameObject->getPosition());
	sprite.setRotation(gameObject->getRotation().y);
	spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setSprite(const sre::Sprite& sprite) {
	this->sprite = sprite;
}

sre::Sprite SpriteComponent::getSprite() {
	return sprite;
}

void SpriteComponent::cleanComponent() {

}