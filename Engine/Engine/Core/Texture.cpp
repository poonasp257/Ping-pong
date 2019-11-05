#include "stdafx.h"

Texture::Texture() : texture(nullptr) {

}

Texture::~Texture() {

}

bool Texture::initialize(ID3D11Device* device, const WCHAR* filename) {
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &texture, NULL);
	if (FAILED(result)) return false;

	return true;
}

ID3D11ShaderResourceView* Texture::getTexture() {
	return texture;
}