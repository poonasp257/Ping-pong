#include "stdafx.h"

CPUManager::CPUManager() {
	PDH_STATUS status;

	canReadCpu = true;

	status = PdhOpenQuery(NULL, 0, &queryHandle);
	if (status != ERROR_SUCCESS) canReadCpu = false;

	status = PdhAddCounter(queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0,
		&counterHandle);
	if (status != ERROR_SUCCESS) canReadCpu = false;

	lastSampleTime = GetTickCount();
	cpuUsage = 0;
}

CPUManager::~CPUManager() {
	if (canReadCpu) PdhCloseQuery(queryHandle);
}

void CPUManager::frame() {
	if (!canReadCpu
		|| (lastSampleTime + 1000) < GetTickCount()) return;
	
	lastSampleTime = GetTickCount();
	PdhCollectQueryData(queryHandle);

	PDH_FMT_COUNTERVALUE value;
	PdhGetFormattedCounterValue(counterHandle, PDH_FMT_LONG, NULL, &value);
	cpuUsage = value.longValue;
}

int CPUManager::getCPUPercentage() const {
	int usage;

	if (canReadCpu) usage = (int)cpuUsage;
	else usage = 0;

	return usage;
}