#include "stdafx.h"

InputManager::InputManager() {

}

InputManager::~InputManager() {
	keyboard->Unacquire();
	mouse->Unacquire();
	directInput->Release();
}

void InputManager::Initiailize(HINSTANCE hInstance, HWND hwnd) {
	DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, NULL);

	directInput->CreateDevice(GUID_SysKeyboard,
		&keyboard, NULL);

	directInput->CreateDevice(GUID_SysMouse,
		&mouse, NULL);

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}

void InputManager::DetectInput(HWND hwnd) {
	keyboard->Acquire();
	mouse->Acquire();

	mouseLastState = mouseCurrentState;

	mouse->GetDeviceState(sizeof(mouseCurrentState), &mouseCurrentState);
	keyboard->GetDeviceState(sizeof(keyCurrentState), (LPVOID)&keyCurrentState);
}

bool InputManager::GetKey(BYTE key) {
	BYTE pressedKey = keyCurrentState[key];

	if (pressedKey & 0x80) {
		keyLastState[key] = pressedKey;
		return true;
	}

	keyLastState[key] = NULL;
	return false;
}

bool InputManager::GetKeyDown(BYTE key) {
	BYTE pressedKey = keyCurrentState[key];

	if (pressedKey & 0x80) {
		if (keyLastState[key] == pressedKey) return false;

		keyLastState[key] = pressedKey;
		return true;
	}

	keyLastState[key] = NULL;
	return false;
}

bool InputManager::GetKeyUp(BYTE key) {
	BYTE pressedKey = keyCurrentState[key];

	if (pressedKey & 0x80) {
		keyLastState[key] = pressedKey;
		return false;
	}
	else {
		if (keyLastState[key] == pressedKey) {
			keyLastState[key] = NULL;
			return true;
		}
	}
	
	return false;
}

bool InputManager::isValidButton(int button) const {
	switch (button) {
	case 0:	case 1: case 2:
		return true;
	default:
		return false;
	}
}

bool InputManager::GetMouseButton(int button) {
	if (!isValidButton(button)) return false;
	
	if (mouseCurrentState.rgbButtons[button] & 0x80) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}

bool InputManager::GetMouseButtonDown(int button) {
	if (!isValidButton(button)) return false;

	if ((mouseCurrentState.rgbButtons[button] & 0x80)
		&& !(mouseLastState.rgbButtons[button] & 0x80)) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}

bool InputManager::GetMouseButtonUp(int button) {
	if (!isValidButton(button)) return false;
	
	if (!(mouseCurrentState.rgbButtons[button] & 0x80)
		&& (mouseLastState.rgbButtons[button] & 0x80)) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}