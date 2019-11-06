#include "stdafx.h"
#include "BounceBall.h"

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

	transform->setPosition(pos + dir * 0.1f);
}

void BounceBall::OnCollisionEnter(Collider *collider) {
	sound->PlayWaveFile(-1000, false);
	moveDir = transform->getPosition() - collider->transform->getPosition();
	moveDir.y = 0;
}

void BounceBall::OnCollisionStay(Collider *collider) {

}

void BounceBall::OnCollisionExit(Collider *collider) {

}