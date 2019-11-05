#ifndef MANASCENE_H
#define MANASCENE_H

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class MainScene : public Scene {
private:
	bool render();

private:
	std::vector<ModelClass*> m_Models;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<SkyBox> skyBox;
	std::unique_ptr<Camera> mainCamera;
	LightShader *m_LightShader;
	TextureShader *m_TextureShader;


	LightClass* m_Light;
	TextClass* m_Text;

public:
	MainScene();
	~MainScene();

	bool initialize(int, int, HWND);
	bool frame(int, float, int);
};
#endif