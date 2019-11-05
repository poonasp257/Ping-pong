#ifndef FONT_H
#define FONT_H

class Font {
private:
	struct FontType {
		float left, right;
		int size;
	};

	struct VertexType {
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};
	
private:
	std::unique_ptr<FontType, std::default_delete<FontType[]>> font;
	std::shared_ptr<Texture> texture;

private:
	bool loadFontData(const char*);
	bool loadTexture(ID3D11Device*, const WCHAR*);

public:
	Font();
	~Font();

	bool initialize(ID3D11Device*, const char*, const WCHAR*);

	ID3D11ShaderResourceView* getTexture();

	void buildVertexArray(void*, const char*, float, float);
};
#endif