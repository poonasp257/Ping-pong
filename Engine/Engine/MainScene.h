#ifndef MANASCENE_H
#define MANASCENE_H

/*
Light class
ShaderManager
*/

#include "modelclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class MainScene : public Scene {
public:
	MainScene();
	MainScene(const MainScene&);
	~MainScene();

	bool initialize(int, int, HWND);
	bool frame(int, float, int);

private:
	bool render();

private:
	Camera* m_Camera;
	std::vector<ModelClass*> m_Models;
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	LightClass* m_Light;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	SkyBox	*m_skyBox;
};
#endif