#include "stdafx.h"

FPSManager::FPSManager() : fps(0), count(0), startTime(0) {
	startTime = timeGetTime();
}

FPSManager::~FPSManager() {

}

void FPSManager::frame() {
	++count; 

	if (timeGetTime() >= (startTime + 1000)) {
		fps = count;
		count = 0;
		startTime = timeGetTime();
	}
}