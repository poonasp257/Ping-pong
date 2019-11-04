#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define Input (InputManager::Instance())

class InputManager {
private:
	BYTE keyCurrentState[256];
	BYTE keyLastState[256];
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	DIMOUSESTATE mouseCurrentState;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 directInput;

private:
	bool isValidButton(int button) const;

public:
	InputManager();
	~InputManager();
		 
	bool GetKey(BYTE key);
	bool GetKeyDown(BYTE key);
	bool GetKeyUp(BYTE key);

	LONG GetAxisX() const { return mouseCurrentState.lX; }
	LONG GetAxisY() const { return mouseCurrentState.lY; }

	bool GetMouseButton(int button);
	bool GetMouseButtonDown(int button);
	bool GetMouseButtonUp(int button);

	void Initiailize(HINSTANCE hInstance, HWND hwnd);

	void DetectInput(HWND hwnd);

	static InputManager& Instance() {
		static InputManager instance;
		return instance;
	}
};

#endif