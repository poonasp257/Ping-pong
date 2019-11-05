#include "stdafx.h"

Transform::Transform(GameObject *gameObject, const D3DXVECTOR3& pos = { 0.0f, 0.0f, 0.0f },
	const D3DXVECTOR3& rotation = { 0.0f, 0.0f, 0.0f }, 
	const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f }) : Component(gameObject, this),
	position(pos), rotation(rotation), scale(scale) {

}

Transform::~Transform() {
}
