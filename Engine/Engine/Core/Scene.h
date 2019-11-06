#ifndef SCENE_H
#define SCENE_H

class Direct3D;

class Scene {
protected:
	std::unique_ptr<Direct3D> direct3D;

protected:
	virtual bool render() = 0;

public:
	Scene();
	virtual ~Scene();

	virtual bool initialize(bool isFullScreen, int screenWidth, int screenHeight, HWND hwnd) = 0;
	virtual bool frame(int fps, float frameTime, int cpu) = 0;
};
#endif