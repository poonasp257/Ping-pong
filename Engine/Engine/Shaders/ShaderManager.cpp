#include "stdafx.h"

ShaderManager::ShaderManager() :
	textureShader(std::make_unique<TextureShader>()), 
	fontShader(std::make_unique<FontShader>()),
	lightShader(std::make_unique<LightShader>()) {

}

ShaderManager::~ShaderManager() {

}

bool ShaderManager::initialize(ID3D11Device *device, HWND hwnd) {
	bool result;

	result = textureShader->initialize(device, hwnd);
	if (!result) return false;

	result = fontShader->initialize(device, hwnd);
	if (!result) return false;

	result = lightShader->initialize(device, hwnd);
	if (!result) return false;

	return true;
}