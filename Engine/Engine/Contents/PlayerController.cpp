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
	D3DXVECTOR3 dir = { 0.0f, 0.0f, 0.0f };

	if (Input.GetKey(DIK_W)) {
		dir += { 0.0f, 0.0f, 1.0f };
	}
	if (Input.GetKey(DIK_A)) {
		dir += { -1.0f, 0.0f, 0.0f };
	}
	if (Input.GetKey(DIK_S)) {
		dir += { 0.0f, 0.0f, -1.0f };
	}
	if (Input.GetKey(DIK_D)) {
		dir += { 1.0f, 0.0f, 0.0f };
	}

	D3DXVECTOR3 movePos = transform->getPosition() + dir * 0.5f;
	transform->setPosition(movePos);
}