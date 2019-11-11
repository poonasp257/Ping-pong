#include "stdafx.h"

BoxCollider::BoxCollider(GameObject* gameObject, Transform* transform) 
	: Collider(gameObject, transform) {
	CreateBoundingVolume();
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

void BoxCollider::CreateBoundingVolume() {
	Model *model = gameObject->getModel();
	auto vertices = model->getVertices();
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

	AdjustBoxScale();
	AdjustBoxRotation();

	minVertex += transform->getPosition();
	maxVertex += transform->getPosition();
	oldPosition = transform->getPosition();
}

void BoxCollider::AdjustBoxScale() {
	minVertex.x *= transform->scale.x;
	minVertex.y *= transform->scale.y;
	minVertex.z *= transform->scale.z;

	maxVertex.x *= transform->scale.x;
	maxVertex.y *= transform->scale.y;
	maxVertex.z *= transform->scale.z;
}

void BoxCollider::AdjustBoxRotation() {
	float radianX = D3DXToRadian(transform->rotation.x);
	float radianY = D3DXToRadian(transform->rotation.y);
	float radianZ = D3DXToRadian(transform->rotation.z);
	
	float x1 = minVertex.x, x2 = maxVertex.x;
	float y1 = minVertex.y, y2 = maxVertex.y;
	float z1 = minVertex.z, z2 = maxVertex.z;

	if (radianZ != 0) {
		minVertex.x = (cos(radianZ) * x1) + (-sin(radianZ) * y1);
		minVertex.y = (sin(radianZ) * x1) + (cos(radianZ) * y1);

		maxVertex.x = (cos(radianZ) * x2) + (-sin(radianZ) * y2);
		maxVertex.y = (sin(radianZ) * x2) + (cos(radianZ) * y2);
	}

	if (radianY != 0) {
		minVertex.x = (cos(radianY) * x1) + (-sin(radianY) * z1);
		minVertex.z = (sin(radianY) * x1) + (cos(radianY) * z1);

		maxVertex.x = (cos(radianY) * x2) + (-sin(radianY) * z2);
		maxVertex.z = (sin(radianY) * x2) + (cos(radianY) * z2);
	}

	if (radianX != 0) {
		minVertex.y = (cos(radianX) * y1) + (-sin(radianX) * z1);
		minVertex.z = (sin(radianX) * y1) + (cos(radianX) * z1);
								 				
		maxVertex.y = (cos(radianX) * y2) + (-sin(radianX) * z2);
		maxVertex.z = (sin(radianX) * y2) + (cos(radianX) * z2);
	}
	
	D3DXVECTOR3 temp = minVertex;
	minVertex.x = min(minVertex.x, maxVertex.x);
	minVertex.y = min(minVertex.y, maxVertex.y);
	minVertex.z = min(minVertex.z, maxVertex.z);

	maxVertex.x = max(temp.x, maxVertex.x);
	maxVertex.y = max(temp.y, maxVertex.y);
	maxVertex.z = max(temp.z, maxVertex.z);
}