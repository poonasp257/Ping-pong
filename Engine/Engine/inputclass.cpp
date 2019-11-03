////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass() {

}

InputClass::~InputClass() {
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
}

bool InputClass::InitDirectInput(HINSTANCE hInstance, HWND hwnd) {
	HRESULT hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&DirectInput, NULL);

	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard, NULL);

	hr = DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse, NULL);

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void InputClass::DetectInput(HWND hwnd) {
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	mouseLastState = mouseCurrentState;

	DIMouse->GetDeviceState(sizeof(mouseCurrentState), &mouseCurrentState);
	DIKeyboard->GetDeviceState(sizeof(keyCurrentState), (LPVOID)&keyCurrentState);
}

bool InputClass::GetKey(BYTE key) {
	BYTE pressedKey = keyCurrentState[key];

	if (pressedKey & 0x80) {
		keyLastState[key] = pressedKey;
		return true;
	}

	keyLastState[key] = NULL;
	return false;
}

bool InputClass::GetKeyDown(BYTE key) {
	BYTE pressedKey = keyCurrentState[key];

	if (pressedKey & 0x80) {
		if (keyLastState[key] == pressedKey) return false;

		keyLastState[key] = pressedKey;
		return true;
	}

	keyLastState[key] = NULL;
	return false;
}

bool InputClass::GetKeyUp(BYTE key) {
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

bool InputClass::IsValidButton(int button) const {
	switch (button) {
	case 0:	case 1: case 2:
		return true;
	default:
		return false;
	}
}

bool InputClass::GetMouseButton(int button) {
	if (!IsValidButton(button)) return false;
	
	if (mouseCurrentState.rgbButtons[button] & 0x80) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}

bool InputClass::GetMouseButtonDown(int button) {
	if (!IsValidButton(button)) return false;

	if ((mouseCurrentState.rgbButtons[button] & 0x80)
		&& !(mouseLastState.rgbButtons[button] & 0x80)) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}

bool InputClass::GetMouseButtonUp(int button) {
	if (!IsValidButton(button)) return false;
	
	if (!(mouseCurrentState.rgbButtons[button] & 0x80)
		&& (mouseLastState.rgbButtons[button] & 0x80)) {
		mouseLastState.rgbButtons[button] = mouseCurrentState.rgbButtons[button];
		return true;
	}

	return false;
}