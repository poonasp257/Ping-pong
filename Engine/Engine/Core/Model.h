#ifndef MODEL_H
#define MODEL_H
class Model {
private:
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct FaceType {
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	struct ModelType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};	

private:
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount, polygonCount;
	std::shared_ptr<Texture> texture;
	std::unique_ptr<ModelType[]> model;

private:
	bool initializeBuffers(ID3D11Device*);
	void renderBuffers(ID3D11DeviceContext*);

	bool loadTexture(ID3D11Device*, const WCHAR*);

	bool createPrimitive(ID3D11Device*);

	bool readFileCounts(const WCHAR*, int&, int&, int&);
	bool loadModel(const WCHAR*);
	
public:
	Model();
	~Model();

	bool initialize(ID3D11Device*, const WCHAR*);
	bool initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void render(ID3D11DeviceContext*);

	int getIndexCount() const { return indexCount; }
	int getVertexCount() const { return vertexCount; }
	int getPolygonCount() const { return polygonCount; }
	ID3D11ShaderResourceView* getTexture() { return texture->getTexture(); }
	ModelType* getVertices() { return model.get(); }
};
#endif