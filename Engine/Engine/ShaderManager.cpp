#include "stdafx.h"

ShaderManager::ShaderManager() :
	textureShader(std::make_unique<TextureShader>()), 
	fontShader(std::make_unique<FontShader>()),
	lightShader(std::make_unique<LightShader>()) {

}

ShaderManager::~ShaderManager() {

}

bool ShaderManager::initialize(ID3D11Device* device, HWND hwnd, D3DXMATRIX	baseViewMatrix) {
	bool result;

	result = textureShader->initialize(device, hwnd);
	if (!result) return false;

	result = fontShader->initialize(device, hwnd);
	if (!result) return false;

	result = lightShader->initialize(device, hwnd);
	if (!result) return false;

	return true;
}

bool ShaderManager::renderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture) {
	return textureShader->render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}

bool ShaderManager::renderFontShader(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 color) {
	return fontShader->render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, color);
}

bool ShaderManager::renderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
	D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor,
	D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower) {
	return lightShader->render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection,
		ambientColor, diffuseColor, cameraPosition, specularColor, specularPower);
}