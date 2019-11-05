#ifndef SYSTEM_H
#define SYSTEM_H

#define WIN32_LEAN_AND_MEAN

class FPSManager;
class CPUManager;
class Timer;
class MainScene;

class System {
private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
	bool isFullScreen;

	std::unique_ptr<Scene>	scene;
	std::unique_ptr<FPSManager> fpsManager;
	std::unique_ptr<CPUManager> cpuManager;
	std::unique_ptr<Timer>		timer;

private:
	void initializeWindows(int&, int&);
	bool frame();

public:
	System();
	~System();

	bool initialize();
	void run();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System *ApplicationHandle = 0;

#endif