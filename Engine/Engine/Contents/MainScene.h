#ifndef MANASCENE_H
#define MANASCENE_H

class MainScene : public Scene {
private:
	bool render();

private:
	std::vector<Model*> m_Models;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::unique_ptr<ShaderManager> shaderManager;
	std::unique_ptr<SkyBox> skyBox;
	std::unique_ptr<Camera> mainCamera;
	std::unique_ptr<Light> light;
	std::unique_ptr<Text> text;

public:
	MainScene();
	~MainScene();

	bool initialize(bool, int, int, HWND);
	bool frame(int, float, int);
};
#endif