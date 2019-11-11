#include "stdafx.h"

ShaderManager::ShaderManager() :
	textureShader(new TextureShader), 
	fontShader(new FontShader),
	directionalLightShader(new DirectionalLightShader),
	pointLightShader(new PointLightShader) {

}

ShaderManager::~ShaderManager() {

}

bool ShaderManager::initialize(ID3D11Device *device, HWND hwnd) {
	bool result;

	result = textureShader->initialize(device, hwnd);
	if (!result) return false;

	result = fontShader->initialize(device, hwnd);
	if (!result) return false;

	result = directionalLightShader->initialize(device, hwnd);
	if (!result) return false;

	result = pointLightShader->initialize(device, hwnd);
	if (!result) return false;

	return true;
}