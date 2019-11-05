#include "stdafx.h"

Transform::Transform(GameObject *gameObject, const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rotation, 
	const D3DXVECTOR3& scale) : Component(gameObject, this),
	position(pos), rotation(rotation), scale(scale) {

}

Transform::~Transform() {
}
