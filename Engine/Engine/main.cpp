#include "stdafx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	auto system = std::make_unique<System>();
	
	bool result = system->initialize();
	if (result) system->run();
	
	return 0;
}