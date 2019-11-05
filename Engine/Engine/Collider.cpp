#include "stdafx.h"
#include "Collider.h"

Collider::Collider(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform) {

}

Collider::~Collider() {

}
