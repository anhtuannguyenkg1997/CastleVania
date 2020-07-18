#pragma once
#include <Windows.h>


/*
Lớp trừu tượng để xác định trình xử lý sự kiện bàn phím
*/
class CKeyEventHandler
{
public:
	//Trạng thái của phím
	virtual void KeyState(BYTE* state) = 0;

	//Một phím được nhấn xuống
	virtual void OnKeyDown(int KeyCode) = 0;


	//Một phím được thả ra
	virtual void OnKeyUp(int KeyCode) = 0;
};


typedef CKeyEventHandler* LPKEYEVENTHANDLER;