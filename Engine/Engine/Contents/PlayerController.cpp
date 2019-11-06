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

	if (Input.GetKeyDown(DIK_A)) {
		dir = { -1.0f, 0.0f, 0.0f };
	}
	else if (Input.GetKeyDown(DIK_D)) {
		dir = { 1.0f, 0.0f, 0.0f };
	}

	transform->setPosition(dir);
}