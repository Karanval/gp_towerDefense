#pragma once

#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Component.hpp"

// Component for handling physics on GameObjects.
class PhysicsComponent : public Component {
public:
	explicit PhysicsComponent(GameObject* gameObject);
	virtual ~PhysicsComponent();
	void initCircle(b2BodyType type, float radius, glm::vec2 center, float density, bool isBullet);
	void initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density, bool isBullet);
	void initRectangle(b2BodyType type, float hx, float hy, glm::vec2 center, float density, bool isBullet);

	void addForce(glm::vec2 force);     // Force gradually affects the velocity over time

	void addImpulse(glm::vec2 impulse);   // Instantly affects velocity

	void setLinearVelocity(glm::vec2 velocity);
	void setPosition(glm::vec2 newPos);

	glm::vec2 getLinearVelocity();

	bool isSensor();

	void setSensor(bool enabled);

	// Move to position in physics scale
	// Internally implemented by setting linearVelocity to delta value
	// This will make objects move until setLinearVelocity({0,0}) is called.
	void moveTo(glm::vec2 pos);

	// Get position in physics scale
	glm::vec2 getPosition();
	void applyBlastImpulse(b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

private:
	b2PolygonShape* polygon = nullptr;
	b2CircleShape* circle = nullptr;
	b2Body* body = nullptr;
	b2Shape::Type shapeType;
	b2Fixture* fixture = nullptr;
	b2BodyType rbType;
	std::vector<PhysicsComponent*> collidingBodies;
	b2World* world = nullptr;

	friend class TowerDefense;
};