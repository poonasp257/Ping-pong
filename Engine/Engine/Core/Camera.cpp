#include "stdafx.h"

Camera::Camera() : sensitivity(0.005f), moveSpeed(0.3f),
	yaw(0.0f), pitch(0.0f), roll(0.0f),
	position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f),
	velocity(0.0f, 0.0f, 0.0f) {

}

Camera::~Camera() {

}

void Camera::setPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::setRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void Camera::move(float x, float y, float z) {
	velocity.x += x;
	velocity.y += y;
	velocity.z += z;
}

void Camera::rotate(float x, float y, float z) {
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

D3DXVECTOR3 Camera::getPosition() const {
	return position;
}


D3DXVECTOR3 Camera::getRotation() const {
	return rotation;
}

void Camera::render() {
	D3DXVECTOR3 defaultForward = { 0.0f, 0.0f, 1.0f };
	D3DXVECTOR3 defaultRight = { 1.0f, 0.0f, 0.0f };
	D3DXMATRIX	rotationMatrix;
	D3DXVECTOR3 up, lookAt;
	
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, 0);
	
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	
	D3DXMATRIX rotateYTempMatrix;
	D3DXMatrixRotationY(&rotateYTempMatrix, yaw);
	
	D3DXVec3TransformCoord(&forward, &defaultForward, &rotateYTempMatrix);
	D3DXVec3TransformCoord(&right, &defaultRight, &rotateYTempMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotateYTempMatrix);
	
	position += velocity.x * right;
	position += velocity.z * forward;

	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 0.4f*3.14f, 800 / 600, 1.0f, 1000.0f);
}

void Camera::frame(float frameTime) {
	float units = moveSpeed * frameTime;

	if (Input.GetKey(DIK_A)) {
		move(-units, 0.0f, 0.0f);
	}
	if (Input.GetKey(DIK_D)) {
		move(units, 0.0f, 0.0f);
	}
	if (Input.GetKey(DIK_W)) {
		move(0.0f, 0.0f, units);
	}
	if (Input.GetKey(DIK_S))  {
		move(0.0f, 0.0f, -units);
	}

	LONG lx = Input.GetAxisX();
	LONG ly = Input.GetAxisY();

	yaw += lx * sensitivity;
	pitch += ly * sensitivity;
}

void Camera::getViewMatrix(D3DXMATRIX& viewMatrix) const {
	viewMatrix = this->viewMatrix;
}

void Camera::getProjectionMatrix(D3DXMATRIX& projtectionMatrix) const {
	projtectionMatrix = this->projectionMatrix;
}