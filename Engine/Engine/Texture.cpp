#include "stdafx.h"

Texture::Texture() : texture(std::make_shared<ID3D11ShaderResourceView>()){

}

Texture::~Texture() {
	texture.reset();
}

bool Texture::initialize(ID3D11Device* device, const WCHAR* filename) {
	HRESULT result;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result)) return false;

	return true;
}

ID3D11ShaderResourceView* Texture::getTexture() {
	return texture.get();
}