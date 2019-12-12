#include "PhysicsComponent.hpp"
#include <Box2D/Box2D.h>
#include <iostream>
#include "TowerDefense.hpp"

PhysicsComponent::PhysicsComponent(GameObject* gameObject)
	: Component(gameObject)
{
	world = TowerDefense::instance->world;
}

PhysicsComponent::~PhysicsComponent() {
	TowerDefense::instance->deregisterPhysicsComponent(this);

	delete polygon;
	delete circle;
	if (body != nullptr && fixture != nullptr) {
		body->DestroyFixture(fixture);
		fixture = nullptr;
	}
	if (world != nullptr && body != nullptr) {
		world->DestroyBody(body);
		body = nullptr;
	}
}

void PhysicsComponent::addImpulse(glm::vec2 impulse) {
	b2Vec2 iForceV{ impulse.x,impulse.y };
	body->ApplyLinearImpulse(iForceV, body->GetWorldCenter(), true);
}

void PhysicsComponent::addForce(glm::vec2 force) {
	b2Vec2 forceV{ force.x,force.y };
	body->ApplyForce(forceV, body->GetWorldCenter(), true);
}

glm::vec2 PhysicsComponent::getLinearVelocity() {
	b2Vec2 v = body->GetLinearVelocity();
	return { v.x,v.y };
}

void PhysicsComponent::setLinearVelocity(glm::vec2 velocity) {
	b2Vec2 v{ velocity.x, velocity.y };
	if (velocity != glm::vec2(0, 0)) {
		body->SetAwake(true);
	}
	body->SetLinearVelocity(v);
}

void PhysicsComponent::setPosition(glm::vec2 newPos) {
	b2Vec2 posi = b2Vec2(newPos.x, newPos.y);
	body->SetTransform(posi, 0);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, glm::vec2 center, float density) {
	assert(body == nullptr);
	// do init
	shapeType = b2Shape::Type::e_circle;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	circle = new b2CircleShape();
	circle->m_radius = radius;
	b2FixtureDef fxD;
	fxD.shape = circle;
	fxD.density = density;
	fixture = body->CreateFixture(&fxD);

	TowerDefense::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density) {
	assert(body == nullptr);
	// do init
	shapeType = b2Shape::Type::e_polygon;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	polygon = new b2PolygonShape();
	polygon->SetAsBox(size.x, size.y, { 0,0 }, 0);
	b2FixtureDef fxD;
	fxD.shape = polygon;
	fxD.density = density;
	fixture = body->CreateFixture(&fxD);

	TowerDefense::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initRectangle(b2BodyType type,float hx, float hy, glm::vec2 center, float density) {
	assert(body == nullptr);
	// do init
	shapeType = b2Shape::Type::e_polygon;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	polygon = new b2PolygonShape();
	polygon->SetAsBox(hx, hy, { 0,0 }, 0);
	b2FixtureDef fxD;
	fxD.shape = polygon;
	fxD.density = density;
	fixture = body->CreateFixture(&fxD);

	TowerDefense::instance->registerPhysicsComponent(this);
}

bool PhysicsComponent::isSensor() {
	return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
	fixture->SetSensor(enabled);
}

void PhysicsComponent::moveTo(glm::vec2 pos) {
	glm::vec2 delta = pos - getPosition();

	setLinearVelocity(delta * (1 / TowerDefense::timeStep));
}

glm::vec2 PhysicsComponent::getPosition() {
	return glm::vec2(body->GetPosition().x, body->GetPosition().y);
}
