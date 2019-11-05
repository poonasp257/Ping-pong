#include "stdafx.h"

Component::Component(GameObject* gameObject, Transform* transform) 
	: gameObject(gameObject), transform(transform) {

}

Component::~Component() {

}