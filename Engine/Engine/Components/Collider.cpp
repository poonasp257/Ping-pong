#include "stdafx.h"

std::vector<Collider*> Collider::colliders;

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform), isEnabled(true) {
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

	Collider *collider = Collide();
	if (!collider) return;

	for (auto& comp : gameObject->components) {
		comp->OnCollisionEnter(collider);
	}
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