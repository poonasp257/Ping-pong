#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
private:
	std::shared_ptr<ID3D11ShaderResourceView> texture;

public:
	Texture();
	~Texture();

	bool initialize(ID3D11Device*, const WCHAR*);

	ID3D11ShaderResourceView* getTexture();
};
#endif