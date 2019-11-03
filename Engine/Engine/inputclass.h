////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>

#define Input (InputClass::instance())

class InputClass {
private:
	BYTE keyCurrentState[256];
	BYTE keyLastState[256];
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseCurrentState;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

private:
	bool IsValidButton(int button) const;

public:
	InputClass();
	~InputClass();
		 
	bool GetKey(BYTE key);
	bool GetKeyDown(BYTE key);
	bool GetKeyUp(BYTE key);

	LONG GetAxisX() const { return mouseCurrentState.lX; }
	LONG GetAxisY() const { return mouseCurrentState.lY; }

	bool GetMouseButton(int button);
	bool GetMouseButtonDown(int button);
	bool GetMouseButtonUp(int button);

	bool InitDirectInput(HINSTANCE hInstance, HWND hwnd);
	void DetectInput(HWND hwnd);

	static InputClass& instance() {
		static InputClass instance;
		return instance;
	}
};

#endif