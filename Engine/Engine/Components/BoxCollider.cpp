#include "stdafx.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform) 
	: Collider(gameObject, transform) {
	minX = transform->getPosition().x - 2.0f;
	minY = transform->getPosition().y - 2.0f;
	minZ = transform->getPosition().z - 2.0f;
	maxX = transform->getPosition().x + 2.0f;
	maxY = transform->getPosition().y + 2.0f;
	maxZ = transform->getPosition().z + 2.0f;
}

BoxCollider::~BoxCollider() {

}

void BoxCollider::start() {

}

void BoxCollider::update() {
	D3DXVECTOR3 pos = transform->getPosition();
	minX = pos.x - 2.0f;
	minY = pos.y - 2.0f;
	minZ = pos.z - 2.0f;
	maxX = pos.x + 2.0f;
	maxY = pos.y + 2.0f;
	maxZ = pos.z + 2.0f;
}

bool BoxCollider::collide(Collider *collider) {
	auto boxCollider = dynamic_cast<BoxCollider*>(collider);
	if (boxCollider) {
		return this->collide(boxCollider);
	}

	return false;
}

bool BoxCollider::collide(BoxCollider *collider) {
	return (this->minX <= collider->maxX && this->maxX >= collider->minX) &&
		(this->minY <= collider->maxY && this->maxY >= collider->minY) &&
		(this->minZ <= collider->maxZ && this->maxZ >= collider->minZ);
}