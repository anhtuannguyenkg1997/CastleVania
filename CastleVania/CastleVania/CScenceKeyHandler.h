#pragma once

#include "CKeyEventHandler.h"
#include "CScene.h"

//Quản lý bàn phím trong một màn chơi
class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	//Con trỏ đến một Scene
	CScene* scence;

public:
	//Set trạng thái của bàn phím
	virtual void KeyState(BYTE* states) = 0;

	//Sự kiện nhấn một phím trên bàn phím
	virtual void OnKeyDown(int KeyCode) = 0;

	//Sự kiện thả phím
	virtual void OnKeyUp(int KeyCode) = 0;

	//Hàm khởi tạo truyền vào một Scene
	CScenceKeyHandler(CScene* s) :CKeyEventHandler() { scence = s; }
};

