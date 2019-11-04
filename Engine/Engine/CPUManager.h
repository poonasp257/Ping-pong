#ifndef CPUMANAGER_H_
#define CPUMANAGER_H_

class CPUManager {
private:
	bool canReadCpu;
	HQUERY queryHandle;
	HCOUNTER counterHandle;
	unsigned long lastSampleTime;
	long cpuUsage;

public:
	CPUManager();
	~CPUManager();

	void frame();

	int getCPUPercentage() const;
};
#endif