#include "stdafx.h"

std::vector<Collider*> Collider::colliders;

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform), isEnabled(true), collider(nullptr) {
	colliders.push_back(this);
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

	auto newCollider = Collide();
	if (!newCollider && !collider) return;

	void(Component::*collisionEvent)(Collider *collider);
	collisionEvent = nullptr;

	if (newCollider && !collider) {
		collisionEvent = &Component::OnCollisionEnter;
	}
	else if (newCollider && collider) {
		collisionEvent = &Component::OnCollisionStay;
	}
	else if (!newCollider && collider) {
		collisionEvent = &Component::OnCollisionExit;
	}

	for (auto& comp : gameObject->components) {
		Component* component = comp.get();
		(component->*collisionEvent)(newCollider);
	}

	collider = newCollider;
}

Collider* Collider::Collide() {
	for (auto collider : colliders) {
		if (collider == this) continue;

		if (this->collide(collider)) {
			return collider;
		}
	}

	return nullptr;
}