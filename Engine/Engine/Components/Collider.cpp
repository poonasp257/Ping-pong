#include "stdafx.h"

std::vector<Collider*> Collider::colliders;

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform), isEnabled(true) {
	colliders.push_back(this);
	detectedColliders.reserve(10);
}

Collider::~Collider() {
	auto found = std::find(colliders.begin(), colliders.end(), this);
	if (found != colliders.end()) {
		colliders.erase(found);
	}
}

void Collider::Start() {

}

void Collider::Update() {
	if (!isEnabled) return;

	this->update();
	auto newColliders = Collide();
	if (newColliders.size() == 0
		&& detectedColliders.size() == 0) return;

	// Exit
	if (newColliders.size() == 0) {
		for (auto it = detectedColliders.begin(); it != detectedColliders.end(); ) {
			for (auto comp : gameObject->components) {
				comp->OnCollisionExit(*it);
			}

			it = detectedColliders.erase(it);
		}

		return;
	}

	for (auto newCollider : newColliders) {
		auto newObj = newCollider->gameObject;
		newObj->transform->position -= newObj->transform->velocity;



		auto found = std::find(detectedColliders.begin(), detectedColliders.end(), newCollider);
		// Enter
		if (found == detectedColliders.end()) {
			for (auto comp : gameObject->components) {
				comp->OnCollisionEnter(newCollider);
			}

			detectedColliders.push_back(newCollider);
			continue;
		}

		// Stay
		for (auto comp : gameObject->components) {
			comp->OnCollisionStay(*found);
		}
	}
}

std::vector<Collider*> Collider::Collide() {
	std::vector<Collider*> newColliders;

	for (auto collider : colliders) {
		if (collider == this) continue;

		if (this->collide(collider)) {
			newColliders.push_back(collider);
		}
	}

	return newColliders;
}