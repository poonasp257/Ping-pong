#include "stdafx.h"
#include "PlayerController.h"

PlayerController::PlayerController(GameObject *gameObject, Transform *transform) 
	: Component(gameObject, transform) {

}

PlayerController::~PlayerController() {

}

void PlayerController::start() {
	
}

void PlayerController::update() {
	this->transform->velocity = { 0, 0, 0 };

	if (Input.GetKey(DIK_W)) {
		transform->velocity += { 0.0f, 0.0f, 1.0f };
	}
	if (Input.GetKey(DIK_A)) {
		transform->velocity += { -1.0f, 0.0f, 0.0f };
	}
	if (Input.GetKey(DIK_S)) {
		transform->velocity += { 0.0f, 0.0f, -1.0f };
	}
	if (Input.GetKey(DIK_D)) {
		transform->velocity += { 1.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 movePos = transform->getPosition() + transform->velocity * 0.5f;
	transform->setPosition(movePos);
}

void PlayerController::OnCollisionEnter(Collider *collider) {
	if (collider->gameObject->tag == "Props") {
		//this->transform->position -= this->transform->velocity;
	}
}