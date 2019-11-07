#include "stdafx.h"

Transform::Transform(GameObject *gameObject, const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rotation,  const D3DXVECTOR3& scale) : Component(gameObject, this),
	position(pos), rotation(rotation), scale(scale) {

}

Transform::~Transform() {

}

void Transform::start() {

}

void Transform::update() {
	D3DXMATRIX rotX, rotY, rotZ;
	
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixRotationZ(&rotZ, D3DXToRadian(rotation.z));
	D3DXMatrixRotationY(&rotY, D3DXToRadian(rotation.y));
	D3DXMatrixRotationX(&rotX, D3DXToRadian(rotation.x));
	D3DXMatrixTranslation(&translationMatrix, position.x, position.y, position.z);

	rotationMatrix = rotZ * rotY * rotX;
	worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}