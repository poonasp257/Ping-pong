#ifndef FPSMANAGER_H
#define FPSMAANGER_H

class FPSManager {
private:
	int fps;
	int count;
	unsigned long startTime;

public:
	FPSManager();
	~FPSManager();

	void frame();

	int getFPS() const { return fps; }
};
#endif