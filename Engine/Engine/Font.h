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
	bool loadFontData(const char*);
	bool loadTexture(ID3D11Device*, const WCHAR*);

private:
	FontType* m_Font;
	TextureClass* m_Texture;

public:
	Font();
	~Font();

	bool initialize(ID3D11Device*, const char*, const WCHAR*);

	ID3D11ShaderResourceView* getTexture() const;

	void buildVertexArray(void*, const char*, float, float);
};
#endif