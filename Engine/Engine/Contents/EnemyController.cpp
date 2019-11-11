#include "stdafx.h"
#include "EnemyController.h"


EnemyController::EnemyController(GameObject *gameObject, Transform *transform)
	: Component(gameObject, transform) {

}

EnemyController::~EnemyController() {

}

void EnemyController::start() {
	ball = GameObject::Find("Ball");
}

void EnemyController::update() {
	transform->position.x = ball->transform->position.x;
}