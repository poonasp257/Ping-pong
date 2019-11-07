#include "stdafx.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform) 
	: Collider(gameObject, transform) {
	Model *model = gameObject->getModel();
	auto *vertices = model->getVertices();
	int vertexCount = model->getVertexCount();

	minVertex = vertices[0].position;
	maxVertex = vertices[0].position;
	for (int i = 0; i < vertexCount; ++i) {
		minVertex.x = min(minVertex.x, vertices[i].position.x);
		minVertex.y = min(minVertex.y, vertices[i].position.y);
		minVertex.z = min(minVertex.z, vertices[i].position.z);
		maxVertex.x = max(maxVertex.x, vertices[i].position.x);
		maxVertex.y = max(maxVertex.y, vertices[i].position.y);
		maxVertex.z = max(maxVertex.z, vertices[i].position.z);
	}

	D3DXVECTOR3 scale = transform->getScale();
	minVertex.x *= scale.x;
	minVertex.y *= scale.y;
	minVertex.z *= scale.z;
	maxVertex.x *= scale.x;
	maxVertex.y *= scale.y;
	maxVertex.z *= scale.z;

	minVertex += transform->getPosition();
	maxVertex += transform->getPosition();
	oldPosition = transform->getPosition();
}

BoxCollider::~BoxCollider() {

}

void BoxCollider::start() {

}

void BoxCollider::update() {
	D3DXVECTOR3 distance = transform->getPosition() - oldPosition;
	minVertex += distance;
	maxVertex += distance;
	oldPosition = transform->getPosition();
}

bool BoxCollider::collide(Collider *collider) {
	auto boxCollider = dynamic_cast<BoxCollider*>(collider);
	if (boxCollider) {
		return this->collide(boxCollider);
	}

	return false;
}

bool BoxCollider::collide(BoxCollider *collider) {
	return (minVertex.x <= collider->maxVertex.x && maxVertex.x >= collider->minVertex.x) &&
		(minVertex.y <= collider->maxVertex.y && maxVertex.y >= collider->minVertex.y) &&
		(minVertex.z <= collider->maxVertex.z && maxVertex.z >= collider->minVertex.z);
}