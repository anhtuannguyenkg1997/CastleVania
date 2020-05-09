#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "CDebug.h"
#include "define.h"
#include "CKeyEventHandler.h"

class CGame
{
	static CGame* _instance;
	HWND hWnd;							// window handle

	LPDIRECT3D9 d3d = NULL;				// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;		// Direct3d device object handle

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;	// sprite helper library

	LPDIRECTINPUT8			di;			// the DirectInput object
	LPDIRECTINPUTDEVICE8	didv;		// the keyboard device

	BYTE keyStates[256];				// DirectInput keyboard state buffer
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];	// buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	ID3DXFont* g_font;					//Font

public:
	void InitKeyboard();
	void Init(HWND hWnd);
	void Draw(int dependOnCamera, int direction, float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	// accordingCam: render theo camera hay là không
	// 0 : no, 1 : yes																																					

	int IsKeyDown(int KeyCode);
	int IsKeyPress(int KeyCode);
	int IsKeyRelease(int KeyCode);
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void ProcessKeyboard();

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return this->backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame* GetInstance();

	ID3DXFont* GetFont() { return this->g_font; }


	~CGame();
};

