#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

class ShaderManager {
private:
	std::unique_ptr<TextureShader> textureShader;
	std::unique_ptr<FontShader> fontShader;
	std::unique_ptr<LightShader> lightShader;

public:
	ShaderManager();
	~ShaderManager();

	bool initialize(ID3D11Device*, HWND);

	TextureShader* const getTextureShader() const { return textureShader.get(); }
	FontShader* const getFontShader() const { return fontShader.get(); }
	LightShader* const getLightShader() const { return lightShader.get(); }
};
#endif