#ifndef BITMAP_H
#define BITMAP_H

class Bitmap {
private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

private:
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	std::shared_ptr<Texture> texture;
	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int previousPosX, previousPosY;
	
private:
	bool initializeBuffers(ID3D11Device*);
	bool updateBuffers(ID3D11DeviceContext*, int, int);
	void renderBuffers(ID3D11DeviceContext*);
	bool loadTexture(ID3D11Device*, const WCHAR*);

public:
	Bitmap();
	~Bitmap();

	bool initialize(ID3D11Device*, int, int, const WCHAR*, int, int);
	bool render(ID3D11DeviceContext*, int, int);

	int getIndexCount() const;
	ID3D11ShaderResourceView* getTexture();

};
#endif