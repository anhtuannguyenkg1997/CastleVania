#pragma once
#include "CGameObject.h"

class Candle : public CGameObject
{
public:
	//Constructor
	Candle(float x, float y, int Type, bool isActive, int State);


	//Lấy vùng bao của Brick
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> &listObjects);

	//Render
	void Render();


	//Set trạng thái của Candle
	void SetState(int state);

	//Lấy vùng bao của Candle
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

