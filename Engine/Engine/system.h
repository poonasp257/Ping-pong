#ifndef SYSTEM_H
#define SYSTEM_H

#define WIN32_LEAN_AND_MEAN

class FPSManager;
class CPUManager;
class Timer;
class MainScene;

class System {
private:
	void initializeWindows(int&, int&);
	void shutdownWindows();
	bool frame();

private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	std::unique_ptr<Scene>	scene;
	std::unique_ptr<FPSManager> fpsManager;
	std::unique_ptr<CPUManager> cpuManager;
	std::unique_ptr<Timer>		timer;

public:
	System();
	System(const System&);
	~System();

	bool initialize();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static System *ApplicationHandle = 0;

#endif