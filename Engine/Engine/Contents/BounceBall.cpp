#include "stdafx.h"
#include "BounceBall.h"
#include "WallController.h"

BounceBall::BounceBall(GameObject* gameObject, Transform* transform)
	: Component(gameObject, transform), moveDir({0, 0, 0}),
	sound(new Sound) {

}

BounceBall::~BounceBall() {
}

void BounceBall::start() {

}

void BounceBall::update() {
	move(moveDir);
}

void BounceBall::loadSound(HWND hwnd, const char *fileName) {
	sound->Initialize(hwnd, fileName);
}

void BounceBall::move(D3DXVECTOR3 dir) {
	D3DXVECTOR3 pos = transform->getPosition();
	transform->setPosition(pos + dir);
}

void BounceBall::OnCollisionEnter(Collider *collider) {
	sound->PlayWaveFile(-1000, false);

	auto otherObj = collider->gameObject;

	if (otherObj->getTag() == "Character") {
		moveDir = transform->getPosition() - collider->transform->getPosition();
		moveDir.y = 0;
	}
	else {
		if (moveDir == D3DXVECTOR3(0, 0, 0)) return;
		
		D3DXVECTOR3 normal = otherObj->transform->getForward();
		moveDir = moveDir + 2 * normal * D3DXVec3Dot(&(-moveDir), &(normal));

		auto wallController = otherObj->GetComponent<WallController>();
		if(wallController) wallController->increaseHitCount();
	}

	D3DXVec3Normalize(&moveDir, &moveDir);
}

void BounceBall::OnCollisionStay(Collider *collider) {

}

void BounceBall::OnCollisionExit(Collider *collider) {

}