#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

class ShaderManager {
public:
	std::unique_ptr<TextureShader> textureShader;
	std::unique_ptr<FontShader> fontShader;
	std::unique_ptr<DirectionalLightShader> directionalLightShader;
	std::unique_ptr<PointLightShader> pointLightShader;

public:
	ShaderManager();
	~ShaderManager();

	bool initialize(ID3D11Device *device, HWND hwnd);
};
#endif