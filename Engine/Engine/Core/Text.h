#ifndef TEXT_H
#define TEXT_H

#define MAX_LENGTH	256

class ShaderManager;

class Text {
private:
	struct SentenceType {
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		int x, y;
		Color color;
	};

	struct VertexType {
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

private:
	std::unique_ptr<Font>		font;
	const ShaderManager			*shaderManager;
	D3DXMATRIX					baseViewMatrix;
	std::vector<SentenceType*>	renderInfo;
	int							screenWidth, screenHeight;
	bool						isEnableRenderInfo;

private:
	bool initializeSentence(SentenceType**, int, int, int, ID3D11Device*);
	bool updateSentence(ID3D11DeviceContext*, SentenceType*, const char*, Color);
	void releaseSentence(SentenceType**);
	bool renderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

public:
	Text(const ShaderManager* shaderManager);
	~Text();

	bool initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool setCameraPosition(const D3DXVECTOR3&, ID3D11DeviceContext*);
	bool setFPS(int, ID3D11DeviceContext*);
	bool setCPU(int, ID3D11DeviceContext*);
	bool setNumOfObjects(int, ID3D11DeviceContext*);
	bool setNumOfPolygons(int, ID3D11DeviceContext*);

	void turnOnOffRenderInfo();
};

#endif