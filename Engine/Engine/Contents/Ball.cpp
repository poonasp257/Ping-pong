#include "stdafx.h"
#include "Ball.h"

Ball::Ball(ID3D11Device *device, ID3D11DeviceContext *deviceContext, const std::string& tag) 
	: GameObject("Ball", tag), device(device), deviceContext(deviceContext) {
	model->initialize(device, L"seafloor.dds");
}

Ball::~Ball() {

}

void Ball::update() {
	model->render(deviceContext);
}