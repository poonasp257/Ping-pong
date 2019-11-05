#ifndef TIMER_H_
#define TIMER_H_

class Timer {
private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;

public:
	Timer();
	~Timer();
	
	bool initialize();
	void frame();

	float getTime() const { return frameTime; };
};
#endif